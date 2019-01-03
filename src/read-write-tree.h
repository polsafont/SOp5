#pragma once

#define MAGIC_NUMBER 0x0133C8F9

void save_tree(rb_tree *tree, char *filename);
rb_tree *load_tree(char *filename);

