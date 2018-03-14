#include <utility>
class IntRangeIterator{
    int point;
public:
    IntRangeIterator(int point ):point(point){}
    void operator++(){
        point++;
    }
    bool operator!=(const IntRangeIterator & ri){
        return ri.point!=point;
    }
    int operator*(){
        return point;
    }
};

class IntRange{
    int start,endpoint;
public:
    IntRange(int endpoint) : endpoint(endpoint){
    }
    IntRange(int start, int endpoint) : start(start),endpoint(endpoint){}
    IntRangeIterator begin()const{return IntRangeIterator(start);}
    IntRangeIterator end()const{return IntRangeIterator(endpoint);}
};

class DoubleRangeIterator{
    double _current, _increment;
public:
    DoubleRangeIterator(double start , double increment = 1):_current(start), _increment(increment){}
    double operator*()const{return _current;}
    void operator++(){_current+=_increment;}
    double operator!=(const DoubleRangeIterator & dri){
        return this->_current != dri._current;
    }
};
class DoubleRange{
    double _start,_end,_increment;
public:
    DoubleRange(double start, double end, double increment):_start(start),_end(end),_increment(increment){}
    DoubleRangeIterator begin()const{
        return DoubleRangeIterator(_start,_increment);
    }
    DoubleRangeIterator end()const{
        return DoubleRangeIterator(_end,_increment);
    }
};
class DoubleMemRange{
        double *_start, *_end;
    public:
        DoubleMemRange( double * start,double*end):_start(start), _end(end){}
        double * begin() const{return _start;};
        double * end() const {return _end;};
};
class IntDoubleRangeIterator{
    int _icurrent, _iincrement;
    double _dcurrent,_dincrement;
public:
    IntDoubleRangeIterator( int icurrent, int iincrement, double dcurrent, double dincrement)
    : _icurrent(icurrent), _iincrement(iincrement), _dcurrent(dcurrent), _dincrement(dincrement)
    {

    }
    void operator++(){
        _icurrent+=_iincrement;
        _dcurrent+=_dincrement;
    }
    std::pair<int, double> operator*(){
        return std::make_pair(_icurrent,_dcurrent);
    }
    bool operator!=(const IntDoubleRangeIterator & idri){
        return this->_icurrent != idri._icurrent;
    }
};
class IntDoubleRange{
    int _istart, _iincrement,_iend;
    double _dstart,_dincrement,_dend;
public:
    IntDoubleRange(int istart, int iend, int iincrement, double dstart,double dend,double dincrement)
    : _istart(istart), _iend(iend), _iincrement(iincrement), _dstart(dstart), _dend(dend), _dincrement(dincrement)
    {
    }
    IntDoubleRangeIterator begin()const{return IntDoubleRangeIterator(_istart,_iincrement,_dstart,_dincrement);}
    IntDoubleRangeIterator end()const{return IntDoubleRangeIterator(_iend,_iincrement,_dend,_dincrement);}
};
