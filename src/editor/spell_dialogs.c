/*
   Editor spell checker dialogs

   Copyright (C) 2012
   The Free Software Foundation, Inc.

   Written by:
   Ilia Maslakov <il.smind@gmail.com>, 2012

   This file is part of the Midnight Commander.

   The Midnight Commander is free software: you can redistribute it
   and/or modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation, either version 3 of the License,
   or (at your option) any later version.

   The Midnight Commander is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <config.h>

#include "editwidget.h"

#ifdef HAVE_ASPELL
#include "spell.h"
#include "spell_dialogs.h"
#endif

/* --------------------------------------------------------------------------------------------- */
/*** public functions ****************************************************************************/
/* --------------------------------------------------------------------------------------------- */

#ifdef HAVE_ASPELL
int
editcmd_dialog_spell_suggest_show (WEdit * edit, const char *word, char **new_word, GArray *suggest)
{

    int sug_dlg_h = 13;         /* dialog height */
    int sug_dlg_w = 29;         /* dialog width */
    int xpos, ypos;
    char *lang_label;
    char *word_label;
    unsigned int i;
    int res;
    char *curr = NULL;
    Dlg_head *sug_dlg;
    WListbox *sug_list;
    int max_btn_len = 0;
    int add_len;
    int replace_len;
    int skip_len;
    int cancel_len;
    WButton *add_btn;
    WButton *replace_btn;
    WButton *skip_btn;
    WButton *cancel_button;

    /* calculate the dialog metrics */
    xpos = (edit->widget.cols - sug_dlg_w) / 2;
    ypos = (edit->widget.lines - sug_dlg_h) * 2 / 3;

    /* Sometimes menu can hide replaced text. I don't like it */
    if ((edit->curs_row >= ypos - 1) && (edit->curs_row <= ypos + sug_dlg_h - 1))
        ypos -= sug_dlg_h;

    add_btn = button_new (3, 27, B_ADD_WORD, NORMAL_BUTTON, _("&Add word"), 0);
    add_len = button_get_len (add_btn);
    replace_btn = button_new (5, 27, B_ENTER, NORMAL_BUTTON, _("&Replace"), 0);
    replace_len = button_get_len (replace_btn);
    skip_btn = button_new (7, 27, B_SKIP_WORD, NORMAL_BUTTON, _("&Skip"), 0);
    skip_len = button_get_len (skip_btn);
    cancel_button = button_new (9, 27, B_CANCEL, NORMAL_BUTTON, _("&Cancel"), 0);
    cancel_len = button_get_len (cancel_button);

    max_btn_len = max (replace_len, skip_len);
    max_btn_len = max (max_btn_len, cancel_len);
    sug_dlg_w += max_btn_len;

    /* create the dialog */
    sug_dlg = create_dlg (TRUE, ypos, xpos, sug_dlg_h, sug_dlg_w,
                          dialog_colors, NULL, NULL, "[ASpell]", _("Check word"), DLG_COMPACT);

    /* create the listbox */
    sug_list = listbox_new (5, 1, sug_dlg_h - 7, 24, FALSE, NULL);
    /* fill the listbox with the suggest */
    for (i = 0; i < suggest->len; i++)
        listbox_add_item (sug_list, LISTBOX_APPEND_AT_END, 0, g_array_index (suggest, char *, i), NULL);

    /* add the dialog */
    add_widget (sug_dlg, sug_list);

    add_widget (sug_dlg, add_btn);
    add_widget (sug_dlg, replace_btn);
    add_widget (sug_dlg, skip_btn);
    add_widget (sug_dlg, cancel_button);

    lang_label = g_strdup_printf ("%s: [%s]", _("Lang"), aspell_get_lang ());
    word_label = g_strdup_printf ("%s: %s", _("Misspelled"), word);

    add_widget (sug_dlg, label_new (1, 1, lang_label));
    add_widget (sug_dlg, label_new (3, 1, word_label));
    add_widget (sug_dlg, groupbox_new (4, 1, sug_dlg_h - 5, 25, _("Suggest")));


    /* pop up the dialog and apply the choosen item */
    res = run_dlg (sug_dlg);
    if (res == B_ENTER)
    {
        char *tmp = NULL;
        listbox_get_current (sug_list, &curr, NULL);

        if (curr != NULL)
            tmp = g_strdup (curr);
        *new_word = tmp;
    }
    /* destroy dialog before return */
    destroy_dlg (sug_dlg);
    g_free (lang_label);
    g_free (word_label);
    return res;
}
#endif  /* HAVE_ASPELL */

/* --------------------------------------------------------------------------------------------- */

#ifdef HAVE_ASPELL
char *
editcmd_dialog_lang_list_show (WEdit * edit, GArray *languages)
{

    int lang_dlg_h = 12;              /* dialog height */
    int lang_dlg_w = 30;              /* dialog width */
    int xpos, ypos;
    char *selected_lang = NULL;
    unsigned int i;
    char *curr = NULL;
    Dlg_head *lang_dlg;
    WListbox *lang_list;

    /* calculate the dialog metrics */
    xpos = (edit->widget.cols - lang_dlg_w) / 2;
    ypos = (edit->widget.lines - lang_dlg_h) / 2;

    /* create the dialog */
    lang_dlg = create_dlg (TRUE, ypos, xpos, lang_dlg_h, lang_dlg_w,
                          dialog_colors, NULL, NULL, "[ASpell]", _("Select language"), DLG_COMPACT);

    /* create the listbox */
    lang_list = listbox_new (1, 1, lang_dlg_h - 3, lang_dlg_w - 2, FALSE, NULL);

    /* add the dialog */
    add_widget (lang_dlg, lang_list);

    /* fill the listbox with the languages */
    for (i = 0; i < languages->len; i++)
        listbox_add_item (lang_list, LISTBOX_APPEND_AT_END, 0, g_array_index (languages, char *, i), NULL);

    /* pop up the dialog and apply the choosen item */
    if (run_dlg (lang_dlg) == B_ENTER)
    {
        listbox_get_current (lang_list, &curr, NULL);

        if (curr != NULL)
            selected_lang = g_strdup (curr);
    }
    /* destroy dialog before return */
    destroy_dlg (lang_dlg);

    return selected_lang;

}
#endif  /* HAVE_ASPELL */

/* --------------------------------------------------------------------------------------------- */