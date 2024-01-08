#pragma once

template<unsigned int N>
class Frame {
public:
	float mValue[N];//value
	float mIn[N]; //in tangent
	float mOut[N]; //out tangent
	float mTime; //frame time
};

typedef Frame<1> ScalarFrame;
typedef Frame<3> VectorFrame;
typedef Frame<4> QuaternionFrame;