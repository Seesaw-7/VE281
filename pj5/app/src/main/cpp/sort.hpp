#ifndef VE281P1_SORT_HPP
#define VE281P1_SORT_HPP

#include <vector>
#include <stdlib.h>
#include <functional>

template<typename T>
void swap(std::vector<T> &vector, long unsigned int i, long unsigned int j){
    auto temp= vector[i];
    vector[i]=vector[j];
    vector[j]=temp;
}

template<typename T, typename Compare>
void bubble_sort(std::vector<T> &vector, Compare comp = std::less<T>()) {
    for(long unsigned int ix=vector.size()-1; ix>0; ix--){
        int swapCnt=0;
        for (long unsigned int jx=0; jx<ix; jx++)
            // if (vector[jx]!=vector[jx+1] && !comp(vector[jx],vector[jx+1])){
            if (comp(vector[jx+1],vector[jx])){
                swap(vector,jx,jx+1);
                swapCnt++;
            }
        if (!swapCnt)
            break;
    }
}

template<typename T, typename Compare>
void insertion_sort(std::vector<T> &vector, Compare comp = std::less<T>()) {
    for (long unsigned int i=1; i<vector.size(); i++){
        if (comp(vector[i],vector[i-1])){
            auto temp=vector[i];
            for (long unsigned int j=i-1; ; j--){
                if (j<i && comp(temp,vector[j]) )
                    vector[j+1]=vector[j];
                else if(j>=i){
                    vector[0]=temp;
                    break;
                }
                else {
                    vector[j+1]=temp;
                    break;
                }
            }
        }
    }
}

template<typename T, typename Compare>
void selection_sort(std::vector<T> &vector, Compare comp = std::less<T>()) {
    for (long unsigned int i=0; i<vector.size()-1; i++){
        long unsigned int index=i;
        for (long unsigned int j=i; j<vector.size()-1; j++){
            // if(vector[index]!=vector[j+1] && !comp(vector[index],vector[j+1]))
            if(comp(vector[j+1],vector[index]))
                index=j+1;
        }
        swap(vector, i, index);
    }
}

template<typename T, typename Compare>
void merge(std::vector<T> &a, std::vector<T> &b, std::vector<T> &C, Compare comp = std::less<T>()) {
    auto ita=a.begin(), itb=b.begin();
    while (ita!=a.end() && itb!=b.end()){
        // if (comp(*ita,*itb) || *ita==*itb)
        if (!comp(*itb,*ita))
            C.push_back(*ita++);
        else
            C.push_back(*itb++);
    }
    if (ita==a.end())
        C.insert(C.end(), itb, b.end());
    else
        C.insert(C.end(), ita, a.end());
}

template<typename T, typename Compare>
void merge_sort(std::vector<T> &vector, Compare comp = std::less<T>()) {
    auto mid=vector.begin()+vector.size()/2;
    if (mid==vector.begin())
        return ;
    //no need to assign, just specify the index
    std::vector<T> left, right;
    left.assign(vector.begin(),mid);
    right.assign(mid,vector.end());
    merge_sort(left, comp);
    merge_sort(right, comp);
    vector.clear();
    merge(left, right, vector, comp);
}

template<typename T, typename Compare>
long int partition(std::vector<T> &vector, long int left, long int right, Compare comp = std::less<T>()){
    long int pivotat=left+(right-left)/2;
    std::vector<T> v2(vector);
    long int l=left, r=right;
    for (long int i=left; i<=right; i++){
        if (comp(v2[i],v2[pivotat]))
            vector[l++]=v2[i];
        else if (comp(v2[pivotat],v2[i]))
            vector[r--]=v2[i];
        else{}
    }
    for (long int i=l; i<=r; i++)
        vector[i]=v2[pivotat];
    return l;
}

template<typename T, typename Compare>
long int in_place_partition(std::vector<T> &vector, long int left, long int right, Compare comp = std::less<T>()){
    // long unsigned int pivotat=left+(right-left)/2;
    // swap(vector, pivotat, left);
    long int l=left+1, r=right;
    while(1){
        while (comp(vector[l], vector[left]))
            l++;
        while (!comp(vector[r], vector[left])&& r!=left)
            r--;
        if (l<r)
            swap(vector, l, r);
        else
            break;
    }
    swap(vector, left, r);
    return r;
}

template<typename T, typename Compare>
void qs_helper(std::vector<T> &vector, long int left, long int right,
               long int (*par) (std::vector<T> &, long int, long int, Compare),
               Compare comp = std::less<T>()){
    long int pivotat;
    if (left>=right)
        return ;
    pivotat=par(vector, left, right, comp);
    qs_helper(vector, left, pivotat-1, par, comp);
    qs_helper(vector, pivotat+1, right, par, comp);
}

template<typename T, typename Compare>
void quick_sort_extra(std::vector<T> &vector, Compare comp = std::less<T>()) {
    qs_helper(vector,0,(long int)vector.size()-1, partition, comp);
}
template<typename T, typename Compare>
void quick_sort_inplace(std::vector<T> &vector, Compare comp = std::less<T>()) {
    qs_helper(vector,0,(long int)vector.size()-1, in_place_partition, comp);
}

#endif //VE281P1_SORT_HPP