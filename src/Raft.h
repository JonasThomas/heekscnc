// Raft.h
/*
 * Copyright (c) 2009, Dan Heeks
 * This program is released under the BSD license. See the file COPYING for
 * details.
 */

#ifndef STABLE_OPS_ONLY
#include "HeeksCNCTypes.h"
//#include "PrintOp.h"
#include "DepthOp.h"
#include "CTool.h"

class CRaft;

class CRaftParams{
public:
	int m_starting_place;
	double m_round_corner_factor;
	double m_material_allowance;
	double m_step_over;
	bool m_keep_tool_down_if_poss;
	bool m_use_zig_zag;
	double m_zig_angle;
	int m_baselayers;
	int m_interfacelayers;
	CTool::ToolNumber_t  m_baselayerextrusion;
	CTool::ToolNumber_t  m_interfacelayerextrusion;
	
	CRaftParams();

    	void set_initial_values(const CTool::ToolNumber_t tool_number);
	void GetProperties(CRaft* parent, std::list<Property *> *list);
	void WriteXMLAttributes(TiXmlNode* pElem);
	void ReadFromXMLElement(TiXmlElement* pElem);
	static wxString ConfigScope() { return(_T("Raft")); }

	bool operator== ( const CRaftParams & rhs ) const;
	bool operator!= ( const CRaftParams & rhs ) const { return(! (*this == rhs)); }
};

class CRaft : public CDepthOp{
public:
	typedef std::list<int> Sketches_t;
	Sketches_t m_sketches;
	CRaftParams m_params;

	static double max_deviation_for_spline_to_arc;

	CRaft():CDepthOp(GetTypeString(), 0, RaftType){}
	CRaft(const std::list<int> &sketches, const int tool_number );
	CRaft(const std::list<HeeksObj *> &sketches, const int tool_number );
	CRaft( const CRaft & rhs );
	CRaft & operator= ( const CRaft & rhs );

	bool operator== ( const CRaft & rhs ) const;
	bool operator!= ( const CRaft & rhs ) const { return(! (*this == rhs)); }

	// HeeksObj's virtual functions
	int GetType()const{return RaftType;}
	const wxChar* GetTypeString(void)const{return _T("Raft");}
	void glCommands(bool select, bool marked, bool no_color);
	const wxBitmap &GetIcon();
	void GetProperties(std::list<Property *> *list);
	HeeksObj *MakeACopy(void)const;
	void CopyFrom(const HeeksObj* object);
	void WriteXML(TiXmlNode *root);
	bool CanAddTo(HeeksObj* owner);
	void GetTools(std::list<Tool*>* t_list, const wxPoint* p);
	void ReloadPointers();

	// COp's virtual functions
	Python AppendTextToProgram(CMachineState *pMachineState);
	void WriteDefaultValues();
	void ReadDefaultValues();

	static HeeksObj* ReadFromXMLElement(TiXmlElement* pElem);

	std::list<wxString> DesignRulesAdjustment(const bool apply_changes);

	static void GetOptions(std::list<Property *> *list);
	static void ReadFromConfig();
	static void WriteToConfig();
};

#endif //#ifndef STABLE_OPS_ONLY
