#include "stdafx.h"
#include "Obj.h"

#include <algorithm>
#include <functional>
#include <set>

template<class T>
inline CObj::ObjPt<T> operator -(const CObj::ObjPt<T>& pt1,const CObj::ObjPt<T>& pt2)
{
	CObj::ObjPt<T> pt;
	pt.value[0] = pt1.value[0] - pt2.value[0];
	pt.value[1] = pt1.value[1] - pt2.value[1];
	pt.value[2] = pt1.value[2] - pt2.value[2];
	return pt;
}

template<class T>
inline CObj::ObjPt<T> operator +(const CObj::ObjPt<T>& pt1,const CObj::ObjPt<T>& pt2)
{
	CObj::ObjPt<T> pt;
	pt.value[0] = pt1.value[0] + pt2.value[0];
	pt.value[1] = pt1.value[1] + pt2.value[1];
	pt.value[2] = pt1.value[2] + pt2.value[2];
	return pt;
}

template<class T>
inline CObj::ObjPt<T> operator *(const CObj::ObjPt<T>& pt,const T& t)
{
	CObj::ObjPt<T> pt1;
	pt1.value[0] = pt.value[0] * t;
	pt1.value[1] = pt.value[1] * t;
	pt1.value[2] = pt.value[2] * t;
	return pt1;
}

template<class T>
inline T abs2(const CObj::ObjPt<T>& pt)
{
	return pt.value[0] * pt.value[0] + pt.value[1] * pt.value[1] + pt.value[2] * pt.value[2];
}

CObj::CObj(void)
	: m_fVolume(0.f)
	, m_Radio(300.f)
	, m_fMaxRadio(300.f)
{
}


CObj::~CObj(void)
{
}


bool CObj::Read(const std::string& pathName)
{
	m_3DPt.clear();
	m_TriIndex.clear();
	std::ifstream is(pathName);
	if(!is.is_open())
		return false;
	std::string text;
	while (std::getline(is,text)) {
		std::istringstream line(text);
		std::string word;
		line >> word;
		if(word == "v") {
			ObjPt<float> a;
			line >> a.value[0] >> a.value[1] >> a.value[2];
			m_3DPt.push_back(a);
		}
		else if (word == "f") {
			ObjPt<unsigned int> a;
			std::string s;
			line >> a.value[0] >> s >> a.value[1] >> s >> a.value[2];
			m_TriIndex.push_back(--a);
		}
	}
	return autopartition();
}

bool CObj::partitionfirst()
{
	if (m_3DPt.empty())
		return false;
	m_PtWeigh = ObjPt<float>(0.f,0.f,0.f);
	for (auto it = m_3DPt.begin(); it != m_3DPt.end(); ++ it) 
		m_PtWeigh += *it;
	m_PtWeigh /= static_cast<float>(m_3DPt.size());
	ObjPt<float> Pt = m_PtWeigh;
	CObj& obj(*this);
	auto It = std::partition(m_TriIndex.begin(),m_TriIndex.end(),
		[Pt,obj](ObjPt<unsigned int>& f) ->bool { return obj.Volume(f,Pt) >= 0.f;});
	std::set<unsigned int> uiset;
	m_PtWeigh2 = Pt;
	for (auto it = m_TriIndex.begin(); it != It; ++ it) {
		uiset.insert(it->value[0]);
		uiset.insert(it->value[1]);
		uiset.insert(it->value[2]);
	}
	if (uiset.empty())
		m_Ptave = m_PtWeigh;
	else {
		m_PtWeigh2 = ObjPt<float>(0.f,0.f,0.f);
		for (auto it = uiset.begin(); it != uiset.end(); ++ it)
			m_PtWeigh2 += m_3DPt[*it];
		if (uiset.size() == m_3DPt.size())
			m_Ptave = m_PtWeigh;
		else {
			m_Ptave = m_PtWeigh * static_cast<float>(m_3DPt.size()) - m_PtWeigh2;
			m_Ptave /= static_cast<float>(m_3DPt.size() - uiset.size());
		}
		m_PtWeigh2 /= static_cast<float>(uiset.size());
	}
	calcVolume(It,false);
	m_fMaxVolume = m_fVolume;
	Draw(It);
	return true;
}
bool CObj::partition(bool bDraw)
{
	if (m_3DPt.empty())
		return false;
	ObjPt<float> Pt = m_PtWeigh2;
	CObj& obj(*this);
	Pt = m_PtWeigh2;
	auto r = m_Radio;
	auto It = std::partition(m_TriIndex.begin(),m_TriIndex.end(),
		[Pt,obj,r](ObjPt<unsigned int>& f) ->bool { return abs2(obj.m_3DPt[f.value[0]] - Pt) <= r;});
	std::set<unsigned int> uiset;
	for (auto it = m_TriIndex.begin(); it != It; ++ it) {
		uiset.insert(it->value[0]);
		uiset.insert(it->value[1]);
		uiset.insert(it->value[2]);
	}
	if (uiset.empty())
		m_Ptave = m_PtWeigh;
	else {
		m_PtWeigh2 = ObjPt<float>(0.f,0.f,0.f);
		for (auto it = uiset.begin(); it != uiset.end(); ++ it)
			m_PtWeigh2 += m_3DPt[*it];
		if (uiset.size() == m_3DPt.size())
			m_Ptave = m_PtWeigh;
		else {
			m_Ptave = m_PtWeigh * static_cast<float>(m_3DPt.size()) - m_PtWeigh2;
			m_Ptave /= static_cast<float>(m_3DPt.size() - uiset.size());
		}
		m_PtWeigh2 /= static_cast<float>(uiset.size());
	}
	calcVolume(It,false);
	if (m_fMaxVolume < m_fVolume) {
		m_fMaxVolume = m_fVolume;
		m_fMaxRadio = m_Radio;
		m_PtMaxWeigh = m_PtWeigh2;
	}
	if (bDraw) Draw(It);
	return true;
}

float CObj::calcVolume(std::vector<ObjPt<unsigned int>>::iterator& It,bool bUnrecalc)
{
	if(bUnrecalc)
		return m_fVolume;
	m_fVolume = 0.f;
	for (auto it = m_TriIndex.begin(); it != It; ++ it)
		m_fVolume += Volume(*it,m_Ptave);
	m_fVolume /= 6.f;
	return m_fVolume;
}

inline float CObj::Volume(const ObjPt<unsigned int>& f,const ObjPt<float>& fPt)const
{
	ObjPt<float> P[3];
	P[0] = m_3DPt[f.value[0]] - fPt;
	P[1] = m_3DPt[f.value[1]] - fPt;
	P[2] = m_3DPt[f.value[2]] - fPt;
	return 
		P[0].value[0] * P[1].value[1] * P[2].value[2] + 
		P[1].value[0] * P[2].value[1] * P[0].value[2] + 
		P[2].value[0] * P[0].value[1] * P[1].value[2] - 
		P[2].value[0] * P[1].value[1] * P[0].value[2] - 
		P[1].value[0] * P[0].value[1] * P[2].value[2] - 
		P[0].value[0] * P[2].value[1] * P[1].value[2];
}

void CObj::Draw(const std::vector<ObjPt<unsigned int>>::iterator& It)
{
	glNewList(1,GL_COMPILE);
	glPushMatrix();
	
	glTranslatef(-m_PtWeigh.value[0],-m_PtWeigh.value[1],-m_PtWeigh.value[2]);

	glColor3f(0.5f,0.5f,0.f);
	glBegin(GL_TRIANGLES);
	for (auto it = m_TriIndex.begin(); it != It; ++ it) {
		glVertex3fv(m_3DPt[it->value[0]].value);
		glVertex3fv(m_3DPt[it->value[1]].value);
		glVertex3fv(m_3DPt[it->value[2]].value);
	}
	glEnd();


	glColor3f(1.f,0.f,0.f);
	glBegin(GL_TRIANGLES);
	for (auto it = It; it != m_TriIndex.end(); ++ it) {
		glVertex3fv(m_3DPt[it->value[0]].value);
		glVertex3fv(m_3DPt[it->value[1]].value);
		glVertex3fv(m_3DPt[it->value[2]].value);
	}
	glEnd();

	glColor3f(1.f,1.f,0.f);
	for (auto it = m_TriIndex.begin(); it != m_TriIndex.end(); ++ it) {
		glBegin(GL_LINE_LOOP);
		glVertex3fv(m_3DPt[it->value[0]].value);
		glVertex3fv(m_3DPt[it->value[1]].value);
		glVertex3fv(m_3DPt[it->value[2]].value);
		glEnd();
	}

	glPointSize(10.0f);
	glBegin(GL_POINTS);
	glColor3f(1.f,0.f,0.f);
	glVertex3fv(m_PtWeigh.value);
	glColor3f(0.f,1.f,0.f);
	glVertex3fv(m_PtWeigh2.value);
	glColor3f(0.f,0.f,1.f);
	glVertex3fv(m_Ptave.value);
	glEnd();
	glPointSize(1.0f);
	glPopMatrix();
	glEndList();
}

std::string CObj::getString(void)
{
	std::string s("就绪");
	std::stringstream ss;
	if (!m_3DPt.empty()){
		ss << "Volume：" << m_fMaxVolume
			<< ",重心(红)："
			<< "(" << m_PtWeigh.value[0] << "," << m_PtWeigh.value[1] << ","  << m_PtWeigh.value[2] << ")"
			<< ",物体重心(绿)："
			<< "(" << m_PtWeigh2.value[0] << ","  << m_PtWeigh2.value[1] << ","  << m_PtWeigh2.value[2] << ")"
			<< ",地面重心(蓝)："
			<< "(" << m_Ptave.value[0] << ","  << m_Ptave.value[1] << ","  << m_Ptave.value[2] << ")"
			<< ",球半径：" << sqrt(m_fMaxRadio);
		ss >> s;
	}
	return s;
}



void CObj::Radio(bool bBig)
{
	if (bBig)
		m_Radio += 50;
	else
		m_Radio -= 50;
	if (m_Radio < 300)
		m_Radio = 300;
	else if (m_Radio > 1500)
		m_Radio = 1500;
	partition();
}


bool CObj::autopartition()
{
	if (m_3DPt.empty())
		return false;
	m_fMaxVolume = 0.f;
	m_PtWeigh = ObjPt<float>(0.f,0.f,0.f);
	for (auto it = m_3DPt.begin(); it != m_3DPt.end(); ++ it) 
		m_PtWeigh += *it;
	m_PtWeigh /= static_cast<float>(m_3DPt.size());
	m_PtWeigh2 = m_PtWeigh;
	//for (m_Radio = 300; m_Radio < 1500; m_Radio += 50)
	//	if(!partition(false))break;
	//for (m_Radio = 1500; m_Radio > 300; m_Radio -= 50)
	//	if(!partition(false))break;
	m_Radio = 1500.f;
	for (int i = 0; i < 10; ++ i)
		if(!partition(false))break;
	for (m_Radio = 1500; m_Radio > 800; m_Radio -= 40)
		if(!partition(false))break;
	m_Radio = m_fMaxRadio;
	m_PtWeigh2 = m_PtMaxWeigh;
	return partition();
}