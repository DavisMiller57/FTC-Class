#include "vex.h"
#include <string>
using std::string;
#include <iostream>
using namespace vex;

// Robot configuration code.

// Brain should be defined by default
brain Brain;

motor Motor1 = motor(PORT20, ratio18_1, false);
motor Motor3 = motor(PORT10, ratio18_1, true);
motor Motor2 = motor(PORT11, ratio18_1, false);
motor Motor4 = motor(PORT1, ratio18_1, true);
motor clawMotor = motor(PORT2, ratio18_1, true);
motor armMotor = motor(PORT3, ratio18_1, false);

controller Controller = controller(primary);

double clawPosition = 0;
double armPosition = 0;

bool isStart = true;

//Simple print method to print a string to the brain screen
void print(string text) {
  Brain.Screen.print(text.c_str());
}
void print(int num) {
  Brain.Screen.print(num);
}

//Sets postion variables to current motor positions
void update(){
  clawPosition = clawMotor.position(degrees);
  armPosition = armMotor.position(degrees);
}

void setTorque(){
  Motor1.setMaxTorque(50, percent);
  Motor2.setMaxTorque(50, percent);
  Motor3.setMaxTorque(50, percent);
  Motor4.setMaxTorque(50, percent);
}

bool clawTo(int pos){
  clawMotor.spinFor(forward, pos, degrees, false);
  return true;
}
bool armTo(int pos){
  armMotor.spinFor(forward, pos, degrees, false);
  return true;
}
// Claw Open
void controller_L1_Pressed(){
  clawMotor.spin(forward, 50, percent);
}

void controller_L1_Released(){
  clawMotor.stop(brake);
}

// Claw Close
void controller_L2_Pressed(){
  clawMotor.spin(reverse, 50, percent);
}

void controller_L2_Released(){
  clawMotor.stop(brake);
}

// Arm Raise
void controller_R1_Pressed(){
  armMotor.spin(forward, 50, percent);
}

void controller_R1_Released(){
  armMotor.stop(brake);
}

// Arm Lower
void controller_R2_Pressed(){
  armMotor.spin(reverse, 50, percent);
}

void controller_R2_Released(){
  armMotor.stop(brake);
}

//Set recieve position
void controller_A_Pressed(){

}
//Set drop position - High Rung
void controller_B_Pressed(){

}
//Set drop position - Low Rung
void controller_X_Pressed(){
  print(armMotor.position(degrees));
}
//Set drop position - Low Basket
void controller_Y_Pressed(){

} 

int main() {
  update();
  setTorque();
  // Create Controller callback events - 15 msec delay to ensure events get registered
  Controller.ButtonL1.pressed(controller_L1_Pressed);
  Controller.ButtonL1.released(controller_L1_Released);
  Controller.ButtonL2.pressed(controller_L2_Pressed);
  Controller.ButtonL2.released(controller_L2_Released);
  Controller.ButtonR1.pressed(controller_R1_Pressed);
  Controller.ButtonR1.released(controller_R1_Released);
  Controller.ButtonR2.pressed(controller_R2_Pressed);
  Controller.ButtonR2.released(controller_R2_Released);
  Controller.ButtonA.pressed(controller_A_Pressed);
  Controller.ButtonB.pressed(controller_B_Pressed); 
  Controller.ButtonX.pressed(controller_X_Pressed);
  Controller.ButtonY.pressed(controller_Y_Pressed);
  wait(15,msec);
  // Configure Arm and Claw motor hold settings and velocity
  //ArmMotor.setStopping(hold);
  //ClawMotor.setStopping(hold);
  //ArmMotor.setVelocity(60, percent);
  //ClawMotor.setVelocity(30, percent);

  // Main Controller loop to set motors to controller axis positions
  while(true){
    // Get the values from the controller axes
    int xAxis = Controller.Axis3.value();
    int yAxis = Controller.Axis4.value();
    int turnAxis = Controller.Axis1.value();
  
    // Combine the X and Y axis values to control the mecanum drivetrain
    Motor1.spin(directionType::fwd, (yAxis + xAxis + turnAxis), velocityUnits::pct);
    Motor2.spin(directionType::fwd, (yAxis - xAxis - turnAxis), velocityUnits::pct);
    Motor3.spin(directionType::fwd, (yAxis - xAxis + turnAxis), velocityUnits::pct);
    Motor4.spin(directionType::fwd, (yAxis + xAxis - turnAxis), velocityUnits::pct);
  
    wait(5, msec);
  }
}