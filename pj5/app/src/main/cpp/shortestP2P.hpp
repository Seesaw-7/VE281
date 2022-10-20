#include<iostream>
#include<list>
#include<vector>
#include<climits>

#define INF INT_MAX

using namespace std;

class ShortestP2P {
public:
    ShortestP2P() {}

    /**
     * read the graph from a vector, the graph is connected and you don't need to consider negative edges.
     *
     * @param vertex_number: the number of vertices in the graph
     * @param edges: a vector representing the edges in the undirected graph,
     *               each element of it is a vector containing three integers,
     *               the first two integers are the two vertices of an edge,
     *               the third integer is the length of the edge
     */

    void setGraph(int vertex_number, vector<vector<int> *> *edges) {
        numV=vertex_number;
        numE=edges->size();
        adjcy_mtrx=new int[numV*numV];

        for (unsigned int i=0; i<numV*numV; i++){
            adjcy_mtrx[i]=INF;
        }
        for (unsigned int i=0; i<numV; i++){
            adjcy_mtrx[numV*i+i]=0;
        }

        int source, destin, length;
        for (unsigned int i=0; i<numE; i++){
            vector<int> * edge=edges->at(i);
            source=edge->at(0);
            destin=edge->at(1);
            length=edge->at(2);
            adjcy_mtrx[numV*source+destin]=length;
            adjcy_mtrx[numV*destin+source]=length;
        }
    }

    /**
     * input: two vertices A and B
     * @param A
     * @param B
     * @return a vector, which contains all the vertices in the shortest path (including A and B)
     */
    vector<int> *distance(unsigned int A, unsigned int B) {
        vector<int>* ans = new vector<int>();

        //Step1
        int* D=new int[numV];//D[v] is the distance estimate for node v
        for (unsigned int i=0; i<numV; i++){
            D[i]=INF;
        }
        D[A]=0;

        int* P=new int[numV];//P[v] is the predecessor for node v
        for (unsigned int i=0; i<numV; i++){
            P[i]=-1;//set P(v) as unknown
        }

        //Step2
        vector<int> R;
        for (unsigned int i=0; i<numV; i++){
            R.push_back(i);//store all the nodes in a set R
        }

        //Step3
        while(!R.empty()){
            //Step3.1
            int node_Dv_min=R[0];//node_Dv_min is the node with minimum distance w.r.t source A
            int index=0;
            for (unsigned int i=0; i<R.size(); i++){
                if (D[node_Dv_min] > D[R[i]]){
                    index=i;
                    node_Dv_min=R[i];
                }
            }
            R.erase(R.begin()+index);
            for (int i=0; i<numV; i++){
                //check whether node i is still in R;
                if (adjcy_mtrx[numV*node_Dv_min+i]== INF)
                    continue;

                bool i_exist=0;
                for(auto it:R){
                    if (it==i){
                        i_exist=1;
                        break;
                    }
                }

                if(!i_exist)
                    continue;;

                if (D[node_Dv_min]+adjcy_mtrx[numV*node_Dv_min+i]<D[i]){
                    D[i]=D[node_Dv_min]+adjcy_mtrx[numV*node_Dv_min+i];
                    P[i]=node_Dv_min;
                }
            }

        }

        while(B!=A){
            ans->push_back(B);
            B=P[B];
        }
        ans->push_back(A);

        delete D;
        delete P;
        return ans;
    };

private:
    int numV, numE;
    int* adjcy_mtrx;
};
