/* $Id: persistent.cpp 4704 2014-01-16 05:30:46Z ming $ */
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
#include "persistent.hpp"

using namespace pj;
using namespace std;


bool PersistentDocument::hasUnread() const
{
    return getRootContainer().hasUnread();
}

string PersistentDocument::unreadName() const throw(Error)
{
    return getRootContainer().unreadName();
}

void PersistentDocument::readNumber(const string &name, double &value) const throw(Error)
{
    return getRootContainer().readNumber(name, value);
}

void PersistentDocument::readBool(const string &name, bool &value) const throw(Error)
{
    return getRootContainer().readBool(name, value);
}

void PersistentDocument::readString(const string &name, string &value) const throw(Error)
{
    return getRootContainer().readString(name, value);
}

void PersistentDocument::readStringVector(const string &name, StringVector &value) const throw(Error)
{
    return getRootContainer().readStringVector(name, value);
}

void PersistentDocument::readObject(PersistentObject &obj) const throw(Error)
{
    getRootContainer().readObject(obj);
}

ContainerNode PersistentDocument::readContainer(const string &name) const throw(Error)
{
    return getRootContainer().readContainer(name);
}

ContainerNode PersistentDocument::readArray(const string &name) const throw(Error)
{
    return getRootContainer().readArray(name);
}

void PersistentDocument::writeNumber(const string &name, double num) throw(Error)
{
    getRootContainer().writeNumber(name, num);
}

void PersistentDocument::writeInt(const string &name, int num) throw(Error)
{
    getRootContainer().writeNumber(name, (double)num);
}

void PersistentDocument::writeBool(const string &name, bool value) throw(Error)
{
    getRootContainer().writeBool(name, value);
}

void PersistentDocument::writeString(const string &name, const string &value) throw(Error)
{
    getRootContainer().writeString(name, value);
}

void PersistentDocument::writeStringVector(const string &name, const StringVector &value) throw(Error)
{
    getRootContainer().writeStringVector(name, value);
}

void PersistentDocument::writeObject(const PersistentObject &obj) throw(Error)
{
    getRootContainer().writeObject(obj);
}

ContainerNode PersistentDocument::writeNewContainer(const string &name) throw(Error)
{
    return getRootContainer().writeNewContainer(name);
}

ContainerNode PersistentDocument::writeNewArray(const string &name) throw(Error)
{
    return getRootContainer().writeNewArray(name);
}

///////////////////////////////////////////////////////////////////////////////

bool ContainerNode::hasUnread() const
{
    return op->hasUnread(this);
}

string ContainerNode::unreadName() const throw(Error)
{
    return op->unreadName(this);
}

void ContainerNode::readNumber(const string &name, double &value) const throw(Error)
{
    return op->readNumber(this, name, value);
}

void ContainerNode::readBool(const string &name, bool &value) const throw(Error)
{
    return op->readBool(this, name, value);
}

void ContainerNode::readString(const string &name, string &value) const throw(Error)
{
    return op->readString(this, name, value);
}

void ContainerNode::readStringVector(const string &name, StringVector &value) const throw(Error)
{
    return op->readStringVector(this, name, value);
}

void ContainerNode::readObject(PersistentObject &obj) const throw(Error)
{
    obj.readObject(*this);
}

ContainerNode ContainerNode::readContainer(const string &name) const throw(Error)
{
    return op->readContainer(this, name);
}

ContainerNode ContainerNode::readArray(const string &name) const throw(Error)
{
    return op->readArray(this, name);
}

void ContainerNode::writeNumber(const string &name, double num) throw(Error)
{
    return op->writeNumber(this, name, num);
}

void ContainerNode::writeInt(const string &name, int num) throw(Error)
{
    return op->writeNumber(this, name, (double)num);
}

void ContainerNode::writeBool(const string &name, bool value) throw(Error)
{
    return op->writeBool(this, name, value);
}

void ContainerNode::writeString(const string &name, const string &value) throw(Error)
{
    return op->writeString(this, name, value);
}

void ContainerNode::writeStringVector(const string &name, const StringVector &value) throw(Error)
{
    return op->writeStringVector(this, name, value);
}

void ContainerNode::writeObject(const PersistentObject &obj) throw(Error)
{
    obj.writeObject(*this);
}

ContainerNode ContainerNode::writeNewContainer(const string &name) throw(Error)
{
    return op->writeNewContainer(this, name);
}

ContainerNode ContainerNode::writeNewArray(const string &name) throw(Error)
{
    return op->writeNewArray(this, name);
}
