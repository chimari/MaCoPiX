void gtkut_tree_view_column_set_markup();
GtkWidget * gtkut_label_new();
GtkWidget * gtkut_frame_new();
void gtkut_frame_set_label();
GtkWidget * gtkut_hbox_new();
GtkWidget * gtkut_vbox_new();
GtkWidget * gtkut_table_new();
void gtkut_table_attach();
void gtkut_table_attach_defaults();
GtkWidget * gtkut_button_new_with_icon();
GtkWidget * gtkut_toggle_button_new_with_icon();
#ifdef USE_GTK3
GtkWidget * gtkut_button_new_from_icon_name();
GtkWidget * gtkut_toggle_button_new_from_icon_name();
GtkWidget * gtkut_image_menu_item_new_with_label();;
#else
GtkWidget * gtkut_button_new_from_stock();
GtkWidget * gtkut_toggle_button_new_from_stock();
#endif
GtkWidget * gtkut_button_new_from_pixbuf();
GtkWidget * gtkut_toggle_button_new_from_pixbuf();
void gtkut_pos();
GtkWidget * gtkut_menu_item_new_with_icon();
void gdkut_flush();
GtkWidget * gtkut_hscale_new();
GtkWidget * gtkut_arrow_new();
void my_cairo_set_source_rgba();
GtkWidget * gtkut_dialog_add_button();

enum{
  POS_START,
  POS_CENTER,
  POS_END
};

typedef enum{
  MY_ARROW_LEFT,
  MY_ARROW_RIGHT,
  MY_ARROW_UP,
  MY_ARROW_DOWN
} MyArrowDirect;

