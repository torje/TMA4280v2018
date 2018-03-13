
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
    DoubleRangeIterator(double start , double increment=1):_current(start), _increment(increment){}
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
