/* $Id: types.cpp 4742 2014-02-12 05:18:31Z bennylp $ */
/*
 * Copyright (C) 2013 Teluu Inc. (http://www.teluu.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include "types.hpp"
#include <cstdio>

using namespace pj;
using namespace std;

#define THIS_FILE   "types.cpp"

///////////////////////////////////////////////////////////////////////////////

Error::Error()
: status(PJ_SUCCESS), srcLine(0)
{
}

Error::Error( pj_status_t prm_status,
          const string &prm_title,
          const string &prm_reason,
          const string &prm_src_file,
          int prm_src_line)
: status(prm_status), title(prm_title), reason(prm_reason),
  srcFile(prm_src_file), srcLine(prm_src_line)
{
}

string Error::info(bool multi_line) const
{
    string output;

    if (status==PJ_SUCCESS) {
        output = "No error";
    } else if (!multi_line) {
        char temp[80];

        if (!title.empty()) {
            output += title + " error: ";
        }
        snprintf(temp, sizeof(temp), " (status=%d)", status);
        output += reason + temp;
        if (!srcFile.empty()) {
            output += " [";
            output += srcFile;
            snprintf(temp, sizeof(temp), ":%d]", srcLine);
            output += temp;
        }
    } else {
        char temp[80];

        if (!title.empty()) {
            output += string("Title:       ") + title + "\n";
        }

        snprintf(temp, sizeof(temp), "%d\n", status);
        output += string("Code:        ") + temp;
        output += string("Description: ") + reason + "\n";
        if (!srcFile.empty()) {
            snprintf(temp, sizeof(temp), ":%d\n", srcLine);
            output += string("Location:    ") + srcFile + temp;
        }
    }

    return output;
}