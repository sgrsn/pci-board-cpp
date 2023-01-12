
#define PERIODIC_TIME_CONTROL 1000 // 実行周期　μs
#define PERIODIC_TIME_LOG 10000 // 実行周期　μs

// Experimental parameter
#define MOTOR_INPUT_LIMIT_TORQUE 0.10
#define MOTOR_INPUT_LIMIT_SPEED 3000  // rpm

// PID
#define SPEED_PID_CONTROLLER_KP 2.72E-5
#define SPEED_PID_CONTROLLER_KI 1.36E-3
#define SPEED_PID_CONTROLLER_KD 0.000

// motor
#define MOTOR_INPUT_LIMIT_VOLTAGE 5.0
#define MOTOR_DAC_CHANNEL 1
#define DAC_OFFSET_VOLTAGE 0.06
#define MOTOR_VOLTAGE_TO_TORQUE 0.0272
#define MOTOR_TORQUE_TO_VOLTAGE 36.764706

// brake
#define BRAKE_INPUT_LIMIT_VOLTAGE 5.0
#define BRAKE_DAC_CHANNEL 2

// torque sensor
#define TORQUESENSOR_OFFSET 0.0876