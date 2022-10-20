#include <algorithm>
#include <vector>
#include <iostream>
using namespace std;

typedef struct point{
    long int x;
    long int y;
}p;

void swap(vector<p> &vector,  long int i, long int j){
    auto temp= vector[i];
    vector[i]=vector[j];
    vector[j]=temp;
}

void input_points(vector<p> &all, long int num){
    for (long int i=0; i<num; i++){
        long int x, y;
        cin>>x>>y;
        all.push_back({x,y});
    }    
}

void find_p0(vector<p> &all){
    long int index=0;
    for (long int i=0; i<(long int)all.size(); i++){
        if (all[i].y<all[index].y)
            index=i;
        else if (all[i].y==all[index].y){
            if (all[i].x<all[index].x)
                index=i;
        }
        else{}
    }
    swap(all, index, 0);    
}

template<typename T, typename Compare>
void std_sort(vector<T> &vector, Compare comp = less<T>()) {
    sort(vector.begin(), vector.end(), comp);
}

long int ccw(const p &p1, const p &p2, const p &p3){
    return ((p2.x-p1.x)*(p3.y-p1.y)-(p2.y-p1.y)*(p3.x-p1.x));
}

bool compare_p(const p &p0, const p &p1, const p&p2){
    if (ccw(p0, p1, p2)>0)
        return 1;
    if ((ccw(p0, p1, p2)==0)){
        if (p1.x!=p2.x || p1.y!= p2.y){
            if (abs(p1.x-p0.x)<abs(p2.x-p0.x) || abs(p1.y-p0.y)<abs(p2.y-p0.y))
                return 1;
        }
        else
            return 1;
    }
    return 0;      
}

struct Compareless{
    p p0;
    bool operator()(const p &p1, const p& p2) const{
        return compare_p(p0, p1, p2);
    }
};

void erase_same(vector<p> &all){
    if (all.size()<2)
        return ;
    if (all.size()==2){
        if (all[0].x==all[1].x && all[0].y==all[1].y)
            all.erase(all.begin());
    }
    for (auto i=all.end()-1; i>all.begin()+1; i--){
        if (ccw(all[0],*i, *(i-1))==0)
            all.erase(i-1);  
    }
    if (all[0].x==all[1].x && all[0].y==all[1].y)
        all.erase(all.begin());
}

void scan(vector<p> &all, vector<p> &hull){
    for (auto it:all){
        while(hull.size()>1 && ccw(*(hull.end()-2), *(hull.end()-1), it)<=0){
            hull.pop_back();
        }
        hull.push_back(it);
    }
}

void print(vector<p> &all){
    for(auto it: all){
        cout<<it.x<<' '<<it.y<<endl;
    }
}

int main(){
    long int num;
    vector<p> allPoints;
    vector<p> hullStack;
    Compareless c;

    cin>>num;
    if (num==0)
        return 0;
    input_points(allPoints, num);

    find_p0(allPoints);
    c.p0=allPoints[0];    
    std_sort(allPoints, c);
    erase_same(allPoints);
    scan(allPoints,hullStack);

    print(hullStack);
}