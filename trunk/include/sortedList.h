#ifndef SORTED_LIST_DOT_H
#define SORTED_LIST_DOT_H

#include <iostream>

template <class T>
class SortedList
{

  // class element within class list
  class SortedListElement
  {
  public:
    T elem;
    SortedListElement* next;
  };

 public:
  /* construcror */
  inline SortedList():list(NULL),size_v(0)
    {}
  inline void freeList()
  {
    /* same as the destructor */
    for (unsigned int i=0;i<size_v;i++)
      /* remove element one by one */
      this->pop_front();
  }


  inline unsigned int size() const {return size_v;}
  void printList() const;
  SortedList& insert (T item);
  SortedList& pop_front ();

  T& operator[] (const unsigned int nIndex) ;

 private:
  SortedListElement* list;
  unsigned int size_v;
};



#include "sortedList.hpp"

#endif
