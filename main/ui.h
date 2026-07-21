#ifndef UI_H
#define UI_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void    ui_init(void);
int32_t ui_get_slider_value(void);
void    ui_set_arc_value(int32_t v);

#ifdef __cplusplus
}
#endif

#endif
