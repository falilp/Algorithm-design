// ###### Config options ################

#define PRINT_PATHS 1

// #######################################

#define BUILDING_DEF_STRATEGY_LIB 1
               
#include "../simulador/Asedio.h"
#include "../simulador/Defense.h"

#ifdef PRINT_PATHS
#include "ppm.h"
#endif

using namespace Asedio;

Vector3 cellCenterToPosition(int i, int j, float cellWidth, float cellHeight){ 
    return Vector3((j * cellWidth) + cellWidth * 0.5f, (i * cellHeight) + cellHeight * 0.5f, 0); 
}

void DEF_LIB_EXPORTED calculateAdditionalCost(float** additionalCost, int cellsWidth, int cellsHeight, float mapWidth, float mapHeight, List<Object*> obstacles, List<Defense*> defenses){
    float cellWidth = mapWidth / cellsWidth;
    float cellHeight = mapHeight / cellsHeight;

    for(int i=0; i<cellsHeight; ++i){
        for(int j=0; j<cellsWidth; ++j) {
            Vector3 cellPosition = cellCenterToPosition(i,j,cellWidth,cellHeight);
            float cost = 10;

            List<Object*>::iterator indexObstacles = obstacles.begin();
            while(indexObstacles != obstacles.end()){
                if(_distance(cellPosition,(*indexObstacles)->position) < (*indexObstacles)->radio){
                    cost *= 2;
                }else{
                    cost /= 2;
                }

                ++indexObstacles;
            }

            List<Defense*>::iterator indexDefenses = defenses.begin();
            while(indexDefenses != defenses.end()){
                if(_distance(cellPosition,(*indexDefenses)->position) < (*indexDefenses)->radio){
                    cost *= 2;
                }else{
                    cost /= 2;
                }

                ++indexDefenses;
            }

            /*
            if((i+j)%2 == 0){
                cost = cellWidth * 100;
            }
            */

            additionalCost[i][j] = cost;
        }
    }
}

void DEF_LIB_EXPORTED calculatePath(AStarNode* originNode, AStarNode* targetNode, int cellsWidth, int cellsHeight, float mapWidth, float mapHeight, float** additionalCost, std::list<Vector3> &path){
    //int maxIter = 100;
    bool condition = false;
    std::vector<AStarNode*> openNode, closedNode;
    std::make_heap(openNode.begin(),openNode.end());

    AStarNode* current = originNode;
    current->parent = nullptr;
    current->G = 0;
    current->H = additionalCost[(int)(current->position.x-(cellsWidth * 0.5f))/cellsWidth][(int)(current->position.y-(cellsHeight * 0.5f))/cellsHeight];
    current->F = current->G + current->H;

    openNode.push_back(current);

    while(!condition && openNode.size() != 0/*maxIter>0*/){//open.size() != 0  [ open != ∅ ]
        current = openNode.back();
        openNode.pop_back();
        closedNode.push_back(current);
        //float min = INF_F;
	    //AStarNode* o = NULL;

        if(targetNode == current){
            condition = true;
        }else{
            for(List<AStarNode*>::iterator index = current->adjacents.begin(); index!=current->adjacents.end(); ++index){
                float dist = _sdistance((*index)->position, targetNode->position);

                if(std::find(closedNode.begin(),closedNode.end(),*index) == closedNode.end()){
                    if(std::find(openNode.begin(),openNode.end(),*index) == openNode.end()){
                        (*index)->parent = current;
                        (*index)->G = current->G + _distance(current->position,(*index)->position);
                        (*index)->H = additionalCost[(int)(current->position.x-(cellsWidth * 0.5f))/cellsWidth][(int)(current->position.y-(cellsHeight * 0.5f))/cellsHeight];
                        (*index)->F = (*index)->G + (*index)->H;
                        openNode.push_back(*index);
                        //std::push_heap(openNode.begin(),openNode.end());
                    }else{
                        float dist = _distance(current->position,(*index)->position);
                        if((*index)->G > (dist+current->G)){
                            (*index)->parent = current;
                            (*index)->G = current->G + dist;
                            (*index)->F = (*index)->G + (*index)->H;
                            //std::push_heap(openNode.begin(),openNode.end());
                        }
                    }
                    std::push_heap(openNode.begin(),openNode.end());
                }
            }
        }
        //--maxIter;
    }
    if(condition){
        std::list<Vector3> pathInverse;
        pathInverse.push_back(targetNode->position);
        current = targetNode->parent;

        while(current != originNode){   
            pathInverse.push_back(current->position);
            current = current->parent;
        }

        pathInverse.push_back(originNode->position);
        path.insert(path.end(),pathInverse.rbegin(),pathInverse.rend());
    }
}