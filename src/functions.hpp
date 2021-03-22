#include "main.h"

lv_obj_t * createBtn(lv_obj_t * parent, lv_coord_t x, lv_coord_t y, lv_coord_t width, lv_coord_t height, int id, const char * title);
lv_style_t * createBtnStyle(lv_style_t * copy, lv_color_t rel, lv_color_t pr, lv_color_t tglRel, lv_color_t tglPr, lv_color_t tglBorder, lv_color_t textColor);
void setBtnStyle(lv_style_t * btnStyle, lv_obj_t * btn);