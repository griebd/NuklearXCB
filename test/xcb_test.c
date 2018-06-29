#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "nuklear_cfg.h"
#include "../nuklear_xcb.h"



int main (void) {
    struct nk_context ctx;
    struct nk_xcb_context *xcb_ctx;
    struct nk_color background;
    enum {EASY, HARD};
    static int op = EASY;
    static char text_buffer[256];
    static struct nk_text_edit text_edit;
    static bool init_text_edit = false;
    if (!init_text_edit) {
	    init_text_edit = true;
	    nk_textedit_init_fixed(&text_edit, text_buffer, 256);
    }


    background = nk_rgb (0, 0, 0);
    xcb_ctx = nk_xcb_init (&background, NULL, 20, 20, 600, 500);
    nk_init_default (&ctx, nk_xcb_default_font (xcb_ctx));
    nk_xcb_set_nk_context (xcb_ctx, &ctx);

    while (1)
    {
	int w, h;

	if (!nk_xcb_handle_event (xcb_ctx))
	{
	    break;
	}

	nk_xcb_size (xcb_ctx, &w, &h);
	if (nk_begin (&ctx, "Menu", nk_rect (0, 0, w, h),
		    NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BACKGROUND))
	{
	    nk_menubar_begin (&ctx);
	    nk_layout_row_begin (&ctx, NK_STATIC, 25, 3);
	    nk_layout_row_push (&ctx, 45);
	    if (nk_menu_begin_label (&ctx, "File", NK_TEXT_LEFT, nk_vec2 (120, 200)))
	    {
		nk_layout_row_dynamic (&ctx, 25, 1);
		if (nk_menu_item_label (&ctx, "Exit", NK_TEXT_LEFT))
		{
		    nk_window_close (&ctx, "Menu");
		}
		nk_menu_end (&ctx);
	    }
	    nk_menubar_end (&ctx);
	    nk_layout_row_static(&ctx, 30, 80, 2);
	    nk_button_label(&ctx, "button");
	    nk_label(&ctx, "another", NK_TEXT_LEFT);
	    nk_layout_row_dynamic(&ctx, 30, 2);
	    if (nk_option_label(&ctx, "easy", op == EASY)) op = EASY;
	    if (nk_option_label(&ctx, "hard", op == HARD)) op = HARD;
	    nk_layout_row_dynamic(&ctx, 25, 1);
	    nk_edit_buffer(&ctx, NK_EDIT_FIELD, &text_edit, nk_filter_default);


	}
	nk_end (&ctx);
	if (nk_window_is_hidden (&ctx, "Menu"))
	{
	    break;
	}

	nk_xcb_render (xcb_ctx);
	nk_clear (&ctx);
    }

    nk_xcb_free (xcb_ctx);
    nk_free (&ctx);

    return EXIT_SUCCESS;
}
