/*#pragma once

#include <initializer_list>
#include "EZ-Template/PID.hpp"
#include "DC_Lib/Motor.hpp"
#include "DC_Lib/Motor_Group.hpp"
#include "EZ-Template/util.hpp"
#include "pros/motor_group.hpp"

namespace DC{
    class MotorPID{
        public:
        DC::DC_MotorGroup mmotors;
        pros::MotorGroup sensors;
        std::vector<double> pidvals = {0,0,0,0};
        std::vector<double> temppidvals = {0,0,0,0};
        std::vector<double> motorpidvals = {18,11,0,0};
        ez::PID ezPID;
        pros::controller_digital_e_t ManUp;
        pros::controller_digital_e_t ManDown;
        pros::controller_digital_e_t AutoUp;
        pros::controller_digital_e_t AutoDown;
        bool manual_override = false;
        std::vector<double> PIDPos;
        int maxPos = -1;
        int PIDIndex = 0;
        double manSpeed = 650;



        MotorPID(std::initializer_list<std::int8_t> m, std::initializer_list<std::int8_t> s, std::initializer_list<double> pid) 
        : mmotors(m), sensors(s)
        {
            pidvals = pid;
            ezPID.constants_set(pidvals[0], pidvals[1],pidvals[2], pidvals[3]);
            ezPID.exit_condition_set(80, 50, 300, 150, 500, 500);
        }

        

    void setManualButtons(pros::controller_digital_e_t up, pros::controller_digital_e_t down){
        ManUp = up;
        ManDown = down;
    }
    void setAutoButtons(pros::controller_digital_e_t up, pros::controller_digital_e_t down){
        AutoUp = up;
        AutoDown = down;
    }

    void setAutoPositions(std::vector<double> pos){
        PIDPos = pos;
        maxPos = static_cast<int>(PIDPos.size()) - 1;
        PIDIndex = 0;
    }

    void setManSpeed(double speed){
        manSpeed = speed;
    }

    void setMotorPowerPID(double v = 0, double p= 0, double i = 0, double d = 0){
        motorpidvals[0] = v;
        motorpidvals[1] = p;
        motorpidvals[2] = i;
        motorpidvals[3] = d;
        mmotors.setHighConstants(motorpidvals[0], motorpidvals[1], motorpidvals[2], motorpidvals[3]);
    }

    void Setmmotors(double input){
        mmotors.setTargetRPM(input);
    };

    double GetPos(){
        return sensors.get_position();
    }

    void TarePos(){
        
        sensors.tare_position_all();
        mmotors.resetPositions();
    }
    
    void ControlTask(){
        pros::delay(2000);
        while(true){
            bool manual_command = false;
            if(master.get_digital_new_press(AutoUp)){
                if(maxPos >= 0){
                    if(PIDIndex < maxPos) PIDIndex++;
                    ezPID.target_set(PIDPos.at(PIDIndex));
                }
            }
            else if(master.get_digital_new_press(AutoDown)){
                if(maxPos >= 0){
                    if(PIDIndex > 0) PIDIndex--;
                    ezPID.target_set(PIDPos.at(PIDIndex));
                }
            }
            else if(master.get_digital(ManUp)){
                manual_override = true;
                ezPID.constants_set(0,0,0,0);
                Setmmotors(manSpeed);
                manual_command = true;
            }
            else if(master.get_digital(ManDown)){
                manual_override = true;
                ezPID.constants_set(0,0,0,0);
                Setmmotors(-manSpeed);
                manual_command = true;
            }else if(manual_override){
                manual_override = false;
                ezPID.constants_set(pidvals[0], pidvals[1],pidvals[2],pidvals[3]);
                ezPID.target_set(GetPos());
            }

            if(!manual_command){
                Setmmotors(ezPID.compute(GetPos()));
            }
            pros::delay(ez::util::DELAY_TIME);
        }
    }

    void Init(){
        mmotors.setBrakeMode(pros::v5::MotorBrake::hold);
        mmotors.setHighConstants(motorpidvals[0], motorpidvals[1], motorpidvals[2], motorpidvals[3]);
        TarePos();
        pros::Task ControlTASK([this] { ControlTask(); });
    }
}; // PID using Motor IME

}*/

#pragma once

#include <initializer_list>
#include "EZ-Template/PID.hpp"
#include "pros/motors.hpp"
#include "pros/motor_group.hpp"
#include "EZ-Template/util.hpp"
#include "pros/motor_group.hpp"

namespace DC{
    class MotorPID{
        public:
        pros::MotorGroup mmotors;
        pros::MotorGroup sensors;
        std::vector<double> pidvals = {0,0,0,0};
        std::vector<double> temppidvals = {0,0,0,0};
        std::vector<double> motorpidvals = {18,11,0,0};
        ez::PID ezPID;
        pros::controller_digital_e_t ManUp;
        pros::controller_digital_e_t ManDown;
        pros::controller_digital_e_t AutoUp;
        pros::controller_digital_e_t AutoDown;
        bool manual_override = false;
        std::vector<double> PIDPos;
        int maxPos = -1;
        int PIDIndex = 0;
        double manSpeed = 650;



        MotorPID(std::initializer_list<std::int8_t> m, std::initializer_list<std::int8_t> s, std::initializer_list<double> pid) 
        : mmotors(m), sensors(s)
        {
            pidvals = pid;
            ezPID.constants_set(pidvals[0], pidvals[1],pidvals[2], pidvals[3]);
            ezPID.exit_condition_set(80, 50, 300, 150, 500, 500);
        }

        

    void setManualButtons(pros::controller_digital_e_t up, pros::controller_digital_e_t down){
        ManUp = up;
        ManDown = down;
    }
    void setAutoButtons(pros::controller_digital_e_t up, pros::controller_digital_e_t down){
        AutoUp = up;
        AutoDown = down;
    }

    void setAutoPositions(std::vector<double> pos){
        PIDPos = pos;
        maxPos = static_cast<int>(PIDPos.size()) - 1;
        PIDIndex = 0;
    }

    void setManSpeed(double speed){
        manSpeed = speed;
    }

    void Setmmotors(double input){
        mmotors.move_velocity(input);
    };

    double GetPos(){
        return sensors.get_position();
    }

    void TarePos(){
        
        sensors.tare_position_all();
        mmotors.tare_position_all();
    }
    
    void ControlTask(){
        pros::delay(2000);
        while(true){
            bool manual_command = false;
            if(master.get_digital_new_press(AutoUp)){
                if(maxPos >= 0){
                    if(PIDIndex < maxPos) PIDIndex++;
                    ezPID.target_set(PIDPos.at(PIDIndex));
                }
            }
            else if(master.get_digital_new_press(AutoDown)){
                if(maxPos >= 0){
                    if(PIDIndex > 0) PIDIndex--;
                    ezPID.target_set(PIDPos.at(PIDIndex));
                }
            }
            else if(master.get_digital(ManUp)){
                manual_override = true;
                ezPID.constants_set(0,0,0,0);
                Setmmotors(manSpeed);
                manual_command = true;
            }
            else if(master.get_digital(ManDown)){
                manual_override = true;
                ezPID.constants_set(0,0,0,0);
                Setmmotors(-manSpeed);
                manual_command = true;
            }else if(manual_override){
                manual_override = false;
                ezPID.constants_set(pidvals[0], pidvals[1],pidvals[2],pidvals[3]);
                ezPID.variables_reset();
                ezPID.target_set(GetPos());
                Setmmotors(0);
                manual_command = true;
            }

            if(!manual_command){
                Setmmotors(ezPID.compute(GetPos()));
            }
            pros::delay(ez::util::DELAY_TIME);
        }
    }

    void Init(){
        mmotors.set_brake_mode_all(pros::v5::MotorBrake::hold);
        TarePos();
        pros::Task ControlTASK([this] { ControlTask(); });
    }
}; // PID using Motor IME

}