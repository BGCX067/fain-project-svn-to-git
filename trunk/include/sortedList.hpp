
template <class T>
void SortedList<T>::printList () const
{
  SortedListElement* tmp=(this)->list;
  std::cout<<"(";
  for (unsigned int i=0;i<(this)->size();i++)
    {
      std::cout<<tmp->elem;
      tmp=tmp->next;
      if (i!=(this)->size()-1)
	std::cout<<",";
    }
  std::cout<<")"<<std::endl;
}

template <class T>
SortedList<T>& SortedList<T>::insert (T e)
{
  SortedListElement* newelem;
  newelem=(SortedListElement*)malloc(sizeof(*newelem));
  newelem->next=NULL;
  newelem->elem=e;

  if (this->list==NULL)
    {
      this->list=newelem;
    }
  else
    {
      SortedListElement* tmp=this->list;
      // looking for where to insert the new element
      // CARE: insertion could be O(log(n)) instead of O(n) 
      // that would requiere a better data structure

      // if the element we are inserting is the lowest
      if((tmp->elem)>e)
	{
	  newelem->next=tmp;
	  this->list=newelem;
	}
      // otherwhise
      else
	{
	  while((tmp->next!=NULL)&&((tmp->next->elem)<e))
	    {
	      tmp=tmp->next;
	    }
	  newelem->next=tmp->next;
	  tmp->next=newelem;
	}
    }
  this->size_v++;
  return (*this);
}

template <class T>
SortedList<T>& SortedList<T>::pop_front()
{
  if ((this->size_v)>0)
    {
      SortedListElement* tmp=this->list;
      this->list=this->list->next;

      free(tmp);

      this->size_v--;
    }
  return (*this); 
}

template <class T>
T& SortedList<T>::operator[] (const unsigned int nIndex)
{
  SortedListElement* tmp=this->list;
  // increment tmp until the wanted element or end of the list
  for (unsigned int i=0;((i<nIndex)&&(i<this->size_v));i++,tmp=tmp->next);
  return tmp->elem;
}
