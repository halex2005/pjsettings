/*
 * PJSIP persistent document implementation based on pugixml backend
 * -----------------------------------------------------------------
 *
 * This library is distributed under the MIT License.
 *
 * Copyright (C) 2014, by Aleksei Kharlov (akharlov@gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __PJSETTINGS_PUGIXML_H__
#define __PJSETTINGS_PUGIXML_H__

#ifndef PJSETTINGS_NOT_USE_PJSUA
#include <pjsua2/persistent.hpp>
#include "pugixml.hpp"

#endif

namespace pjsettings
{
    class PjPugixmlDocument : public pj::PersistentDocument
    {
    public:
        PjPugixmlDocument();
        virtual void loadFile(const std::string &filename) throw(pj::Error);
        virtual void loadString(const std::string &input) throw(pj::Error);
        virtual void saveFile(const std::string &filename) throw(pj::Error);
        virtual std::string saveString() throw(pj::Error);
        virtual pj::ContainerNode &getRootContainer() const;
    private:
        void initRoot();
        pugi::xml_document _document;
        mutable pj::ContainerNode _rootNode;
    };

}

#endif
