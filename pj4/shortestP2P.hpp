#include<iostream>
#include<list>
#include<vector>
#include<climits>
// You are not allowed to include additional libraries, either in shortestP2P.hpp or shortestP2P.cc

#define INF INT_MAX

using namespace std;

class ShortestP2P {
  public:
      ShortestP2P() {}


      /* Read the graph from stdin
       * The input has the following format:
       *
       * Firstline: 1 unsigned int, the total number of verticies, X.
       * Secondline: 1 unsigned int, the total number of edges between vertices, Y.
       * A set of Y lines, each line contains tuple of 2 unsigned int and 1 int (not unsigned int!), in the format of: A(point 1, unsigned) B(point 2, unsigned) dist(distance, int)
       *
       * Example:
       * 4
       * 4
       * 0 1 5
       * 1 2 -1
       * 2 3 4
       * 3 1 6
       * 
       * 
       * 
       *
       * Vertices that are not connected have a infinitly large distance. You may use INF (previously defined at the top of this cope snippet) for infinitly large distance.
       * 
       * Special: when the graph is not proper, where there exist any pair of vertices whose minimum distance does not exist, terminate immediately by printing:
       * cout << "Invalid graph. Exiting." << endl;
       *
       * Note: vertex pairs that are not connected, which have infinitely large distances are not considered cases where "minimum distances do not exist".
       */
      void readGraph(){
        cin>>numV>>numE;
        adjcy_mtrx=new int[numV*numV];
        
        for (unsigned int i=0; i<numV*numV; i++){
          adjcy_mtrx[i]=INF;
        }
        for (unsigned int i=0; i<numV; i++){
          adjcy_mtrx[numV*i+i]=0;
        }

        int source, destin;
        int weight;
        for (unsigned int i=0; i<numE; i++){
          cin>>source>>destin>>weight;
          adjcy_mtrx[numV*source+destin]=weight;
        }

        int ifINF=FloydWarshall();
        if (!ifINF){
          std::cout<<"Invalid graph. Exiting.";
          std::exit(0);
        }

      }


      /* Input: 2 vertices A and B
       * Output: distance between them.
       * cout << dist << endl;
       *
       * When the A and B are not connected print INF:
       * cout << "INF" << endl;
       */
      void distance(unsigned int A, unsigned int B){
        // if (dist[A*numV+B]==INF)
        if (adjcy_mtrx[A*numV+B]==INF)
          cout<<"INF"<<endl;
        else
          cout<<adjcy_mtrx[A*numV+B]<<endl;
        // cout<<(dist[A*numV+B]==INF) ? INF : dist[A*numV+B];
      }

      ~ShortestP2P(){
        delete [] adjcy_mtrx;
        // delete [] dist;
      }

  private:
    // internal data and functions.
    unsigned int numV, numE;
    int* adjcy_mtrx;
    // int* dist;

    //original version of BellmanFord:(O(2V^4))
    // bool BellmanFord(){
    //   dist= new int[numV*numV];
    //   for (unsigned int z=0; z<(numV*numV); z++){
    //     dist[z]=adjcy_mtrx[z];
    //   }

    //   for (unsigned int i=0; i<numV; i++){//for every node i, make i the source and find the distance
    //                             //from i to every other node
    //     for (unsigned int m=0; m<numV-1; m++){
    //       for (unsigned int k=0; k<numV; k++){
    //         for (unsigned int l=0; l<numV; l++){
    //           if (adjcy_mtrx[k*numV+l]!=INF && dist[i*numV+k]!=INF){
    //             if (dist[i*numV+k]+adjcy_mtrx[k*numV+l]<dist[i*numV+l]){
    //               dist[i*numV+l]=dist[i*numV+k]+adjcy_mtrx[k*numV+l];
    //             }
    //           }
    //         }
    //       }
    //     }   

    //     for (unsigned int m=0; m<numV-1; m++){
    //       for (unsigned int k=0; k<numV; k++){
    //         for (unsigned int l=0; l<numV; l++){
    //           if (adjcy_mtrx[k*numV+l]!=INF && dist[i*numV+k]!=INF){
    //             if (dist[i*numV+k]+adjcy_mtrx[k*numV+l]<dist[i*numV+l]){
    //               dist[i*numV+l]=INF;
    //               return 0;
    //             }
    //           }
    //         }
    //       }
    //     }   
    //   }
    //   return 1;
    // }

    //Improved version of BellmanFord(O(V^4))
    // bool BellmanFord(){

    //   for (unsigned int i=0; i<numV; i++){//for every node i, make i the source and find the distance
    //                             //from i to every other node
    //     for (unsigned int m=0; m<numV-1; m++){//iterate numV-1 times
    //       for (unsigned int k=0; k<numV; k++){
    //         for (unsigned int l=0; l<numV; l++){
    //           if (adjcy_mtrx[k*numV+l]!=INF && adjcy_mtrx[i*numV+k]!=INF
    //               && adjcy_mtrx[i*numV+k]+adjcy_mtrx[k*numV+l]<adjcy_mtrx[i*numV+l]){
    //               adjcy_mtrx[i*numV+l]=adjcy_mtrx[i*numV+k]+adjcy_mtrx[k*numV+l]; 
    //           }
    //         }
    //         if (adjcy_mtrx[numV*k+k]<0)
    //           return 0;
    //       }
    //     }   
    //   return 1;
    // }  

    //Improved version O(V^3)  
    bool FloydWarshall(){
      for (unsigned int m=0; m<numV; m++){
        for (unsigned int i=0; i<numV; i++){
          for (unsigned int j=0; j<numV; j++){
            if (adjcy_mtrx[numV*i+m] != INF && adjcy_mtrx[numV*m+j] != INF 
              && adjcy_mtrx[numV*i+m]+adjcy_mtrx[numV*m+j] < adjcy_mtrx[numV*i+j]){
              adjcy_mtrx[numV*i+j]=adjcy_mtrx[numV*i+m]+adjcy_mtrx[numV*m+j];
              }
          }
          if (adjcy_mtrx[numV*i+i]<0){
            return 0;
          }
        }
      } 
      return 1;     
    }
};
