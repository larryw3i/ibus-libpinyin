/* vim:set et ts=4 sts=4:
 *
 * ibus-libpinyin - Intelligent Pinyin engine based on libpinyin for IBus
 *
 * Copyright (c) 2012 Peng Wu <alexepico@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef __PY_TABLE_EDITOR_
#define __PY_TABLE_EDITOR_

#include <sqlite3.h>
#include "PYEditor.h"
#include "PYLookupTable.h"

namespace PY {

class TableDatabase{
public:
    static void init ();
    static TableDatabase & systemInstance (void) { return *m_system_instance; }
    static TableDatabase & userInstance (void)   { return *m_user_instance; }

public:
    TableDatabase();
    ~TableDatabase();

public:
    gboolean isDatabaseExisted(const char *filename);
    gboolean createDatabase(const char *filename);

    gboolean openDatabase(const char *filename, gboolean writable);
    gboolean listPhrases(const char *prefix,
                         std::vector<std::string> & phrases);

    gboolean getPhraseInfo(const char *phrase, int & freq);
    gboolean updatePhrase(const char *phrase, int freq);
    gboolean deletePhrase(const char *phrase, int freq);

    gboolean importTable (const char *filename);
    gboolean exportTable (const char *filename);
    gboolean clearTable ();

private:
    gboolean executeSQL(sqlite3 *sqlite);

private:
    sqlite3 *m_sqlite;
    String m_sql;

private:
    static std::unique_ptr<TableDatabase> m_system_instance;
    static std::unique_ptr<TableDatabase> m_user_instance;
};

class TableEditor : public Editor {
public:
    TableEditor (PinyinProperties &props, Config & config);
    virtual ~TableEditor ();

    virtual gboolean processKeyEvent (guint keyval, guint keycode, guint modifers);
    virtual void pageUp (void);
    virtual void pageDown (void);
    virtual void cursorUp (void);
    virtual void cursorDown (void);
    virtual void update (void);
    virtual void reset (void);
    virtual void candidateClicked (guint index, guint button, guint state);

private:
    gboolean updateStateFromInput (void);

    void clearLookupTable (void);
    void updateLookupTable (void);
    void updatePreeditText (void);
    void updateAuxiliaryText (void);

    gboolean selectCandidateInPage (guint index);
    gboolean selectCandidate (guint index);

    gboolean processSpace (guint keyval);
    gboolean processEnter (guint keyval);

    gboolean removeCharBefore (void);
    gboolean removeCharAfter (void);

    gboolean processLabelKey (guint keyval);
    gboolean processEditKey (guint keyval);
    gboolean processPageKey (guint keyval);

private:
    /* variables */
    LookupTable m_lookup_table;

    String m_preedit_text;
    String m_auxiliary_text;

    TableDatabase *m_table_database;

    const static int m_aux_text_len = 50;
};

};

#endif
