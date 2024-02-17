// ###### Config options ################

//#define PRINT_DEFENSE_STRATEGY 1    // generate map images

// #######################################

#define BUILDING_DEF_STRATEGY_LIB 1

#include "../simulador/Asedio.h"
#include "../simulador/Defense.h"

#ifdef PRINT_DEFENSE_STRATEGY
#include "ppm.h"
#endif

#ifdef CUSTOM_RAND_GENERATOR
RAND_TYPE SimpleRandomGenerator::a;
#endif

using namespace Asedio;

//END
#pragma region Seleccion

Vector3 cellCenterToPosition(int i, int j, float cellWidth, float cellHeight){
    return Vector3((j * cellWidth) + cellWidth * 0.5f, (i * cellHeight) + cellHeight * 0.5f, 0);
}

Vector3 Seleccion(bool** freeCells,float** cellValues, int nCellsWidth, int nCellsHeight, float mapWidth, float mapHeight, float cellWidth, float cellHeight){
    float min = 0;
    int Px = 0;
    int Py = 0;
    for(int i=0; i<nCellsHeight; i++){
        for(int j=0; j<nCellsWidth; ++j){
            if(min < cellValues[i][j] && freeCells[i][j]){
                min = cellValues[i][j];
                Px = i;
                Py = j;
            }
        }
    }

    freeCells[Px][Py] = false;
    return cellCenterToPosition(Px,Py,cellWidth,cellHeight);
}

#pragma endregion

//END
#pragma region Factibilidad 

bool Factibilidad(std::list<Object*> obstacles, std::list<Defense*> defenses,Defense* defensas,Vector3 position,float mapWidth,float mapHeight){
    bool factible = true;

    if(position.x - defensas->radio < 0 || position.y - defensas->radio < 0 || position.x + defensas->radio > mapWidth || position.y + defensas->radio > mapHeight){
        factible = false;
    }

    List<Object*>::iterator IteradorObstaculos = obstacles.begin();
    float dist = 0;

    while(IteradorObstaculos != obstacles.end() && factible == true){
        /*if(position.x == (*IteradorObstaculos)->position.x && position.y == (*IteradorObstaculos)->position.y){
            factible = false;
        }*/
        dist = _distance(position,(*IteradorObstaculos)->position);    

        if(dist == 0 || dist < (defensas->radio+(*IteradorObstaculos)->radio)){
            factible = false;
        }    

        IteradorObstaculos++;
    }

    List<Defense*>::iterator IteradorDefenses = defenses.begin();
    dist = 0;

    while(IteradorDefenses != defenses.end() && factible == true){
        /*if(position.x == (*IteradorDefenses)->position.x && position.y == (*IteradorDefenses)->position.y){
            factible = false;
        }*/
        dist = _distance(position,(*IteradorDefenses)->position);    

        if(dist == 0 || dist < (defensas->radio+(*IteradorDefenses)->radio)){
            factible = false;
        }    

        IteradorDefenses++;
    }

    return factible;
}

#pragma endregion

//END
#pragma region Values

float cellValue(int row, int col, bool** freeCells, int nCellsWidth, int nCellsHeight, float mapWidth, float mapHeight, List<Object*> obstacles, List<Defense*> defenses){
    float cellWidth = mapWidth/nCellsWidth;
    float cellHeight = mapHeight/nCellsHeight;
    float MediaMedidas = (cellWidth+cellHeight)/2;
    int divDimensiones = ((nCellsWidth+nCellsHeight)/2)/2.5;
    float dist = 0;

    Vector3 Centro = cellCenterToPosition(nCellsWidth/2,nCellsHeight/2,cellWidth,cellHeight);
    Vector3 RelativePosition = cellCenterToPosition(row,col,cellWidth,cellHeight);
    
    dist = _distance(Centro,RelativePosition);

    if(divDimensiones/2 >= (int)(dist-(MediaMedidas*0.5f))/MediaMedidas){
        return abs(1000 - (int)(dist-(MediaMedidas*0.5f))/MediaMedidas);
    }else{
        return abs(1000 - (((int)(dist-(MediaMedidas*0.5f))/MediaMedidas)*2));
    }
}    // implemente aqui la funci�n que asigna valores a las celdas

#pragma endregion

//END
#pragma region ValuesTorrest

float cellTorrest(int row, int col, bool** freeCells, int nCellsWidth, int nCellsHeight, float mapWidth, float mapHeight, List<Object*> obstacles, List<Defense*> defenses) {
	float cellWidth = mapWidth/nCellsWidth;
    float cellHeight = mapHeight/nCellsHeight;
    float MediaMedidas = (cellWidth+cellHeight)/2;
    int divDimensiones = ((nCellsWidth+nCellsHeight)/2)/2.5;
    float dist = 0;
    
    Vector3 RelativePosition = cellCenterToPosition(row,col,cellWidth,cellHeight);
    
    dist = _distance((*defenses.begin())->position,RelativePosition);
    
    if(divDimensiones/2 >= (int)((dist-(MediaMedidas*0.5f))/MediaMedidas) && (int)((dist-(MediaMedidas*0.50f))/MediaMedidas) % 2 == 0){
        return abs(1000 - (int)(dist-(MediaMedidas*0.5f))/MediaMedidas);
    }else{
        return abs(1000 - (((int)(dist-(MediaMedidas*0.5f))/MediaMedidas)*2));
    }
}   // implemente aqui la funci�n que asigna valores a las celdas

#pragma endregion

//END
#pragma region BoolCells

void IntroducirCells(bool** Cells,int nCellsWidth, int nCellsHeight, float mapWidth, float mapHeight, std::list<Object*> obstacles){   
    float cellWidth = mapWidth / nCellsWidth;
    float cellHeight = mapHeight / nCellsHeight;    

    auto iteratorObstacles = obstacles.begin();
    
    while(iteratorObstacles != obstacles.end()){
        Cells[(int)(((*iteratorObstacles)->position.x-cellWidth*0.5f)/cellWidth)][(int)(((*iteratorObstacles)->position.y-cellHeight*0.5f)/cellHeight)] = false;

        iteratorObstacles++;
    }
}

#pragma endregion

//END
#pragma region Defenses

void DEF_LIB_EXPORTED placeDefenses(bool** freeCells, int nCellsWidth, int nCellsHeight, float mapWidth, float mapHeight, std::list<Object*> obstacles, std::list<Defense*> defenses){
    float cellWidth = mapWidth / nCellsWidth;
    float cellHeight = mapHeight / nCellsHeight; 
    
    //IntroducirCells(freeCells,nCellsWidth,nCellsHeight,mapWidth,mapHeight,obstacles);
    
    float** cellValues = new float*[nCellsHeight];
    float** cellValuesTorrets = new float*[nCellsHeight];  

    for(int i=0; i<nCellsHeight; ++i){
        cellValues[i] = new float[nCellsWidth];
        //std::cout<<"[ ";
        for(int j=0; j<nCellsWidth; ++j){
            cellValues[i][j] = cellValue(i,j,freeCells,nCellsWidth,nCellsHeight,mapWidth,mapHeight,obstacles,defenses);
            //std::cout<<cellValues[i][j]<<" ";
        }
        //std::cout<<"] "<<std::endl;
    }

    Vector3 auxiliar;
    auto iterator = defenses.begin();

    while(iterator != defenses.end()){
        if(iterator == defenses.begin()){
            auxiliar = Seleccion(freeCells,cellValues,nCellsWidth,nCellsHeight,mapWidth,mapHeight,cellWidth,cellHeight);

            if(Factibilidad(obstacles,defenses,(*iterator),auxiliar,mapWidth,mapHeight)){
                (*iterator)->position = auxiliar;
                iterator++;

                for(int i=0; i<nCellsHeight; ++i){
                    cellValuesTorrets[i] = new float[nCellsWidth];
                    //std::cout<<"[ ";
                    for(int j=0; j<nCellsWidth; ++j){
                        cellValuesTorrets[i][j] = cellTorrest(i,j,freeCells,nCellsWidth,nCellsHeight,mapWidth,mapHeight,obstacles,defenses);
                        //std::cout<<cellValuesTorrets[i][j]<<" ";
                    }
                    //std::cout<<"] "<<std::endl;
                }
            }
        }else{
            auxiliar = Seleccion(freeCells,cellValuesTorrets,nCellsWidth,nCellsHeight,mapWidth,mapHeight,cellWidth,cellHeight);

            if(Factibilidad(obstacles,defenses,(*iterator),auxiliar,mapWidth,mapHeight)){
                (*iterator)->position = auxiliar;
                iterator++;
            }
        }
    }

#ifdef PRINT_DEFENSE_STRATEGY
    dPrintMap("strategy.ppm", nCellsHeight, nCellsWidth, cellHeight, cellWidth, freeCells, cellValues, std::list<Defense*>(), true);

    for(int i = 0; i < nCellsHeight ; ++i)
        delete [] cellValues[i];
        delete [] cellValuesTorrets[i];
	delete [] cellValues;
    delete [] cellValuesTorrets;
	cellValues = NULL;
    cellValuesTorrets = NULL;
#endif
}
#pragma endregion