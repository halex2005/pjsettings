/*
 * PJSIP persistent document implementation based on jsoncpp backend
 * -----------------------------------------------------------------
 *
 * Copyright (C) 2014, by Aleksei Kharlov (akharlov@gmail.com)
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

#ifndef __PJSETTINGS_JSONCPP_H__
#define __PJSETTINGS_JSONCPP_H__

#ifndef PJSETTINGS_NOT_USE_PJSUA
#include <pjsua2/persistent.hpp>
#include "json.h"

#endif

namespace pjsettings
{
    class JsonCppDocument : public pj::PersistentDocument
    {
    public:
        JsonCppDocument(bool notStyledOutputOnWriting = false);
        virtual void loadFile(const std::string &filename) throw(pj::Error);
        virtual void loadString(const std::string &input) throw(pj::Error);
        virtual void saveFile(const std::string &filename) throw(pj::Error);
        virtual std::string saveString() throw(pj::Error);
        virtual pj::ContainerNode &getRootContainer() const;
    private:
        void initRoot();
        Json::Value _document;
        mutable pj::ContainerNode _rootNode;
        bool _notStyledOutputOnWriting;
    };

}

#endif
