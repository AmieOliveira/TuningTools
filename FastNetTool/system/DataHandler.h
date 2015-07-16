

#ifndef FASTNETTOOL_SYSTEM_DATAHANDLER_H
#define FASTNETTOOL_SYSTEM_DATAHANDLER_H
#include <omp.h>
#include <vector>
#include <iostream>
#include <boost/python.hpp>
#include <algorithm>    // std::copy
#include <ctime>
#include "FastNetTool/system/util.h"
#include <boost/python/stl_iterator.hpp>

using namespace std;
namespace py = boost::python;

template <class Type> class DataHandler
{
  private:

    double tictac;
    vector<Type> *vec;
    /// Holds the number of rows the array has.
    unsigned numRows;
    /// Holds the number of collumns the array has.
    unsigned numCols;

  
  public:
  
    ///Default constructor
    DataHandler( py::list data, const unsigned cols ):numCols(cols)
    {
      tictac = 0.0;
      numRows = py::len(data)/numCols; 
      time_t tstart, tend; 
      tstart = time(0);
      vec = new std::vector<Type>();
      vec->resize(numRows*numCols);
      std::copy(boost::python::stl_input_iterator<Type>(data), boost::python::stl_input_iterator<Type>(), vec->begin());
      tend = time(0); 
      tictac = difftime(tend, tstart);
    }

    DataHandler( Type *ptr, const unsigned rows, const unsigned cols ):numCols(cols),numRows(rows)
    {
      tictac = 0.0;
      vec = new vector<Type>();
      vec->resize(numRows*numCols);
      std::copy(ptr, ptr + numRows*numCols, vec->begin());
    }

    ~DataHandler()
    {
      delete vec;
    }

    unsigned size(){  return vec->size();}; 

    /// Set value to vector
    void setValue(const unsigned row, const unsigned col, Type value)
    {
      vec[col + (numCols*row)] = value;
    }

    /// get value from vector
    Type getValue(const unsigned row, const unsigned col)
    {
      return vec[col + (numCols*row)];
    }

    ///Get array pointer
    Type* getPtr()const{return vec->data();}

    ///Get std vector
    vector<Type>* getVecPtr()const{return vec;};


    /// copy vector to extern vector
    void copy( vector<Type> &ref ){
      ref.insert( ref.end(),vec->begin(), vec->end() );
    }


    /// Access the data in the array.
    /**
     This method returns the array value in the specified indexes. This is
     necessary, since the data in the mxArray is stored in a single 1D vector,
     so this method must apply the necessary offset calculations in order to correctly
     access the data. This method can be also used to write data into the array.
     @param[in] row The row index.
     @param[in] col The collumn index.
     @return the array value at the specified position.
    */
    Type &operator()(const unsigned row, const unsigned col) const
    {
      return vec[col + (numCols*row)];
    }
   
        
    /// Return the number of row of the mxArray that we are accessing.
    unsigned getNumRows() const
    {
      return numRows;
    }
    
    
    /// Return the number of collumns of the mxArray that we are accessing.
    unsigned getNumCols() const
    {
      return numCols;
    }
    
    
    ///Print the first 5 rows values into the array. using for debug.
    void showInfo()
    {
      for(unsigned i=0; i < 5; ++i){
        for(unsigned j=0; j < numCols; ++j){
          cout << "[" << i << "][" << j << "] = " << getValue(i,j) << endl;
        }
      }
    }

    //double tictac(){return tictac;};

};

#endif
