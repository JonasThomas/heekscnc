// Reselect.cpp
// Copyright (c) 2010, Dan Heeks
// This program is released under the BSD license. See the file COPYING for details.

#include "stdafx.h"
#include "Reselect.h"
#include "interface/PropertyString.h"
#include "interface/PropertyInt.h"
#include "interface/ObjList.h"

static bool GetSketches(std::list<int>& sketches )
{
	// check for at least one sketch selected

	const std::list<HeeksObj*>& list = heeksCAD->GetMarkedList();
	for(std::list<HeeksObj*>::const_iterator It = list.begin(); It != list.end(); It++)
	{
		HeeksObj* object = *It;
		if(object->GetType() == SketchType)
		{
			sketches.push_back(object->m_id);
		}
	}

	if(sketches.size() == 0)return false;

	return true;
}

void ReselectSketches::Run()
{
	std::list<int> sketches;
	heeksCAD->PickObjects(_("Select Sketches"), MARKING_FILTER_SKETCH);
	if(GetSketches( sketches ))
	{
		heeksCAD->CreateUndoPoint();
		m_sketches->clear();
		*m_sketches = sketches;
		((ObjList*)m_object)->Clear();
		m_object->ReloadPointers();
		heeksCAD->Changed();
	}
	else
	{
		wxMessageBox(_("Select cancelled. No sketches were selected!"));
	}

	// get back to the operation's properties
	heeksCAD->ClearMarkedList();
	heeksCAD->Mark(m_object);
}

static bool GetSolids(std::list<int>& solids )
{
	// check for at least one sketch selected

	const std::list<HeeksObj*>& list = heeksCAD->GetMarkedList();
	for(std::list<HeeksObj*>::const_iterator It = list.begin(); It != list.end(); It++)
	{
		HeeksObj* object = *It;
		if(object->GetType() == SolidType || object->GetType() == StlSolidType)
		{
			solids.push_back(object->m_id);
		}
	}

	if(solids.size() == 0)return false;

	return true;
}

void ReselectSolids::Run()
{
	std::list<int> solids;
	heeksCAD->PickObjects(_("Select Solids"), MARKING_FILTER_SOLID | MARKING_FILTER_STL_SOLID);
	if(GetSolids( solids ))
	{
		heeksCAD->CreateUndoPoint();
		m_solids->clear();
		*m_solids = solids;
		((ObjList*)m_object)->Clear();
		m_object->ReloadPointers();
		heeksCAD->Changed();
	}
	else
	{
		wxMessageBox(_("Select cancelled. No solids were selected!"));
	}

	// get back to the operation's properties
	heeksCAD->ClearMarkedList();
	heeksCAD->Mark(m_object);
}

wxString GetIntListString(const std::list<int> &list)
{
	wxString str;
	int i = 0;
	for(std::list<int>::const_iterator It = list.begin(); It != list.end(); It++, i++)
	{
		if(i > 0)str.Append(_T(" "));
		if(i > 8)
		{
			str.Append(_T("..."));
			break;
		}
		str.Append(wxString::Format(_T("%d"), *It));
	}

	return str;
}

#ifdef OP_SKETCHES_AS_CHILDREN
void AddSolidsProperties(std::list<Property *> *list, HeeksObj* object)
{
	std::list<int> solids;
	for(HeeksObj* child = object->GetFirstChild(); child; child = object->GetNextChild())
	{
		if(child->GetIDGroupType() == SolidType)solids.push_back(child->GetID());
	}
#else
void AddSolidsProperties(std::list<Property *> *list, const std::list<int> &solids)
{
#endif
	if(solids.size() == 0)list->push_back(new PropertyString(_("solids"), _("None"), NULL));
	else if(solids.size() == 1)list->push_back(new PropertyInt(_("solid id"), solids.front(), NULL));
	else list->push_back(new PropertyString(_("solids"), GetIntListString(solids), NULL));
}

#ifdef OP_SKETCHES_AS_CHILDREN
void AddSketchesProperties(std::list<Property *> *list, HeeksObj* object)
{
	std::list<int> sketches;
	for(HeeksObj* child = object->GetFirstChild(); child; child = object->GetNextChild())
	{
		if(child->GetIDGroupType() == SketchType)sketches.push_back(child->GetID());
	}
#else
void AddSketchesProperties(std::list<Property *> *list, const std::list<int> &sketches)
{
#endif
	if(sketches.size() == 0)list->push_back(new PropertyString(_("sketches"), _("None"), NULL));
	else if(sketches.size() == 1)list->push_back(new PropertyInt(_("sketch id"), sketches.front(), NULL));
	else list->push_back(new PropertyString(_("sketches"), GetIntListString(sketches), NULL));
}

