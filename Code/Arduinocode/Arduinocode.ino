#include <Servo.h>  // 引入舵机库
// 引脚定义
#define DIR 4       // 方向引脚
#define motorPWM 5  // PWM引脚
#define CHA 2       // 编码器A相（中断）
#define CHB 3       // 编码器B相
Servo myservo;
// 全局变量
volatile long pulseVal = 0;  // 脉冲计数（正转+，反转-）
bool isMoving = false;       // 是否在运动
int pos = 90;                // 舵机角度变量（新增：接收转向值）
int cmdSpeed = 0;            // 存储解析后的速度
int cmdMode = 0;             // 存储解析后的模式
long cmdDist = 0;            // 存储解析后的距离
bool hasNewCmd = false;      // 新指令标志

void setup() {
  myservo.attach(9);
  // 引脚初始化
  pinMode(DIR, OUTPUT);
  pinMode(motorPWM, OUTPUT);
  pinMode(CHA, INPUT_PULLUP);
  pinMode(CHB, INPUT_PULLUP);

  // 编码器中断（A相变化时计数）
  attachInterrupt(digitalPinToInterrupt(CHA), countPulse, CHANGE);

  // 串口提示（更新输入格式）
  Serial.begin(9600);
  Serial.println("输入格式：转向值,速度,模式,距离（例：90,100,0,0 或 40,50,1,200）");
  Serial.println("说明：转向值直接赋值给舵机角度（会被约束在40~130）");
}

// 简化编码器计数（只保留核心方向判断）
void countPulse() {
  int a = digitalRead(CHA);
  int b = digitalRead(CHB);
  // 正转+1，反转-1（方向反了就交换1和-1）
  pulseVal += (a == b) ? -1 : 1;
}

// 简化电机控制函数
void move(int speed, int mode, long target) {
  int pwm = map(abs(speed), 0, 100, 0, 255);  // 速度转PWM
  digitalWrite(DIR, speed > 0 ? HIGH : LOW);  // 方向控制
  isMoving = true;

  if (mode == 1) {                    // 模式1：定距离停止
    pulseVal = 0;                     // 重置计数
    while (abs(pulseVal) < target) {  // 未到目标就持续运行
      analogWrite(motorPWM, pwm);
      if (Serial.available() > 0) break;  // 新指令打断
    }
    analogWrite(motorPWM, 0);  // 到目标/被打断都停止
    Serial.println("模式1：已停止");
  } else {  // 模式0：持续运行
    analogWrite(motorPWM, pwm);
    Serial.println("模式0：持续运行中");
  }
  isMoving = false;
}

// 串口输入处理函数（新增转向值解析并赋值给pos）
void handleSerialInput() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    // 分割4个参数（转向值,速度,模式,距离）
    int c1 = input.indexOf(',');             // 转向值与速度的分隔
    int c2 = input.indexOf(',', c1 + 1);     // 速度与模式的分隔
    int c3 = input.indexOf(',', c2 + 1);     // 模式与距离的分隔
    if (c1 == -1 || c2 == -1 || c3 == -1) {  // 校验是否有3个逗号
      Serial.println("格式错！需：转向值,速度,模式,距离");
      return;
    }

    // 解析参数（新增转向值解析）
    int steer = input.substring(0, c1).toInt();      // 转向值
    int spd = input.substring(c1 + 1, c2).toInt();   // 速度
    int mode = input.substring(c2 + 1, c3).toInt();  // 模式
    long dist = input.substring(c3 + 1).toInt();     // 距离

    // 简单校验（保持原有风格）
    if (spd < -100 || spd > 100) {
      Serial.println("速度范围：-100~100");
      return;
    }
    if (mode != 0 && mode != 1) {
      Serial.println("模式只能是0或1");
      return;
    }
    if (mode == 1 && dist <= 0) {
      Serial.println("模式1距离需为正整数");
      return;
    }

    // 核心：转向值直接赋值给pos变量（后续会被constrain约束）
    pos = steer;
    pos = map(steer, -100, 0, 0, 90);
    pos = map(steer, 0, 100, 90, 180);
    pos = constrain(pos, 40, 130);

    // 存储其他参数并标记新指令
    cmdSpeed = spd;
    cmdMode = mode;
    cmdDist = dist;
    hasNewCmd = true;
  }
}

void loop() {
  // 约束舵机角度在40~130
  myservo.write(pos);  // 设置舵机角度
  delay(200);          // 缩短延迟，提高响应速度

  // 处理串口输入（解析参数）
  handleSerialInput();

  // 在loop中调用move（当有新指令时）
  if (hasNewCmd) {
    move(cmdSpeed, cmdMode, cmdDist);
    hasNewCmd = false;  // 重置指令标志
  }
}