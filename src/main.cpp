#include "main.h"

#include "globals.hpp"
#include "functions.hpp"

static lv_res_t btn_click_action(lv_obj_t * btn);
void initStyles();

/* LVGL Coords:
--INACCESSABLE/BRAIN INFO--
(0,0)---------------(480,0)
  |						|
  |						|
  |						|
(0,240)-----------(480,240)
*/

static lv_color_t mainCanvasBuffer[32 * 220 * 220]; //size is px size times area(w*h)

lv_obj_t * mainButton;
lv_obj_t * mainCanvas;
lv_obj_t * mainLabel;

lv_style_t style_min_red;
lv_style_t style_min_blue;

std::shared_ptr<ChassisController> drive;

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	master.setText(0, 0, std::to_string(autonomousDefault));

	initStyles();

	mainButton = createBtn(lv_scr_act(), 10, 10, 200, 50, 0, "test");
	lv_btn_set_action(mainButton, LV_BTN_ACTION_CLICK, btn_click_action);
	setBtnStyle(createBtnStyle(&lv_style_plain, LV_COLOR_MAKE(125, 0, 0), LV_COLOR_MAKE(255, 0, 0), LV_COLOR_MAKE(100, 0, 0), LV_COLOR_MAKE(200, 0, 0), LV_COLOR_MAKE(0, 0, 0), LV_COLOR_MAKE(255, 255, 255)), mainButton);

	mainLabel = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_text(mainLabel, "Button has not been clicked.");
	lv_obj_align(mainLabel, NULL, LV_ALIGN_IN_LEFT_MID, 10, 0);
	
	mainCanvas = lv_canvas_create(lv_scr_act(), NULL);
	lv_canvas_set_buffer(mainCanvas, mainCanvasBuffer, 220, 220, LV_IMG_CF_TRUE_COLOR);
	lv_obj_set_pos(mainCanvas, 250, 10);

	lv_canvas_draw_circle(mainCanvas, 110, 110, 10, LV_COLOR_YELLOW);

	Da.setBrakeMode(AbstractMotor::brakeMode::coast);
	Db.setBrakeMode(AbstractMotor::brakeMode::coast);
	Dc.setBrakeMode(AbstractMotor::brakeMode::coast);
	Dd.setBrakeMode(AbstractMotor::brakeMode::coast);

	Da.moveVelocity(0);
	Db.moveVelocity(0);
	Dc.moveVelocity(0);
	Dd.moveVelocity(0);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	while (true) {
		double leftX = master.getAnalog(ControllerAnalog::leftX);	//okapi::conroller master.getAnalog() is double from -1 to 1
		double leftY = master.getAnalog(ControllerAnalog::leftY);	//pros::controller master.get_analog() is -128 to 127
		double rightY = master.getAnalog(ControllerAnalog::rightX);
		double rightX = master.getAnalog(ControllerAnalog::rightY);

		Da.moveVelocity(200 * (rightY + rightX + leftX));
		Db.moveVelocity(200 * (rightY - rightX + leftX));
		Dc.moveVelocity(200 * (-rightY + rightX + leftX));
		Dd.moveVelocity(200 * (-rightY - rightX + leftX));

		lv_canvas_draw_circle(mainCanvas, rightX + 110, rightY + 110, leftY, LV_COLOR_SILVER);

		if(master.getDigital(ControllerDigital::L1)) {
			intake1.moveVoltage(12000);
			intake2.moveVoltage(-12000);
		} else if(master.getDigital(ControllerDigital::L2)) {
			intake1.moveVelocity(-12000);
			intake2.moveVelocity(12000);
		} else {
			intake1.moveVelocity(0);
			intake2.moveVelocity(0);
		}

		if(master.getDigital(ControllerDigital::R1)) {
			intake3.moveVoltage(12000);
		} else if(master.getDigital(ControllerDigital::R2)) {
			intake3.moveVoltage(-12000);
		} else {
			intake3.moveVoltage(0);
		}

		pros::delay(10);
	}
}

// User functions: 

static lv_res_t btn_click_action(lv_obj_t * btn) {
	uint8_t id = lv_obj_get_free_num(btn);

    if(id == 0) {
        char buffer[100];
        sprintf(buffer, "button was clicked %i \nmilliseconds from start \n %i", pros::millis(), master.getAnalog(ControllerAnalog::leftX));
        lv_label_set_text(mainLabel, buffer);
    }
    return LV_RES_OK;
}

void initStyles() {
	lv_style_copy(&style_min_red, &lv_style_plain);
	style_min_red.body.main_color = LV_COLOR_MAKE(200, 0, 0);
	style_min_red.body.radius = 0;
}