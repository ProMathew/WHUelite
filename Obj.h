#pragma once
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
class CObj  
{

private:
	template<class T>
		struct ObjPt {
		ObjPt& operator --() {
			-- value[0];
			-- value[1];
			-- value[2];
			return *this;
		}

		ObjPt& operator ++() {
			++ value[0];
			++ value[1];
			++ value[2];
			return *this;
		}
		ObjPt& operator +=(const ObjPt& pt) {
			value[0] += pt.value[0];
			value[1] += pt.value[1];
			value[2] += pt.value[2];
			return *this;
		}
		ObjPt& operator /=(const T& it) {
			value[0] /= it;
			value[1] /= it;
			value[2] /= it;
			return *this;
		}
		ObjPt() { }
		ObjPt(const T& t1,const T& t2,const T& t3) {
			value[0] = t1;
			value[1] = t2;
			value[2] = t3;
		}
		T value[3];
	};
template<class T>
friend ObjPt<T> operator +(const ObjPt<T>& pt1,const ObjPt<T>& pt2);
template<class T>
friend ObjPt<T> operator -(const ObjPt<T>& pt1,const ObjPt<T>& pt2);
template<class T>
friend T abs2(const ObjPt<T>& pt);
template<class T>
friend ObjPt<T> operator *(const ObjPt<T>& pt,const T& t);
public:
	
	float calcVolume(std::vector<ObjPt<unsigned int>>::iterator& It = std::vector<ObjPt<unsigned int>>::iterator(),bool bUnrecalc = true);
	bool Read(const std::string& pathName);
	CObj();
	virtual ~CObj();
private:
	std::vector<ObjPt<float>> m_3DPt;
	std::vector<ObjPt<unsigned int>> m_TriIndex;
	ObjPt<float> m_PtWeigh;
	ObjPt<float> m_PtWeigh2;
	ObjPt<float> m_Ptave;
	ObjPt<float> m_PtMaxWeigh;
	float m_Radio;
	float m_fVolume;
	float m_fMaxVolume;
	float m_fMaxRadio;
	inline float Volume(const ObjPt<unsigned int>& f,const ObjPt<float>& fPt) const;
	void Draw(const std::vector<ObjPt<unsigned int>>::iterator& It);
	bool partitionfirst();
	bool autopartition();
public:
	std::string getString(void);
	void Radio(bool bBig);
	bool partition(bool bDraw = true);
};

