// ###### Config options ################
// #######################################

#define BUILDING_DEF_STRATEGY_LIB 1

#include "../simulador/Asedio.h"
#include "../simulador/Defense.h"
#include "cronometro.h"
#include <vector>

using namespace Asedio;         

typedef struct CellsMap{
    CellsMap();
    CellsMap(float val,int posiI,int posiJ):value(val),PositionI(posiI),PositionJ(posiJ){}
    float value;
    int PositionI;
    int PositionJ;
    inline bool operator <(const CellsMap& cell){return this->value<cell.value;}
}CellsMap;


Vector3 cellCenterToPosition(int i, int j, float cellWidth, float cellHeight){
    return Vector3((j * cellWidth) + cellWidth * 0.5f, (i * cellHeight) + cellHeight * 0.5f, 0);
}


float defaultCellValue(int row, int col, bool** freeCells, int nCellsWidth, int nCellsHeight, float mapWidth, float mapHeight, List<Object*> obstacles, List<Defense*> defenses) {
    	
    float cellWidth = mapWidth / nCellsWidth;
    float cellHeight = mapHeight / nCellsHeight;

    Vector3 cellPosition((col * cellWidth) + cellWidth * 0.5f, (row * cellHeight) + cellHeight * 0.5f, 0);
    	
    float val = 0;
    for (List<Object*>::iterator it=obstacles.begin(); it != obstacles.end(); ++it) {
	    val += _distance(cellPosition, (*it)->position);
    }

    return val;
}


//////////////////////////////////
#pragma region NoPreorden

#pragma region FreeCells

void FreeCell(bool** freeCells,int nCellsHeight,int nCellsWidth){
    for(int i=0;i<nCellsHeight;++i){
        for(int j=0;j<nCellsWidth;++j){
            freeCells[i][j] = true;
        }
    }
}

#pragma endregion

#pragma region Seleccion

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

#pragma region Factibilidad 

bool Factibilidad(std::list<Object*> obstacles, std::list<Defense*> defenses,Defense* defensas,Vector3 position,float mapWidth,float mapHeight){
    bool factible = true;

    if(position.x - defensas->radio < 0 || position.y - defensas->radio < 0 || position.x + defensas->radio > mapWidth || position.y + defensas->radio > mapHeight){
        factible = false;
    }

    List<Object*>::iterator IteradorObstaculos = obstacles.begin();
    float dist = 0;

    while(IteradorObstaculos != obstacles.end() && factible == true){
        dist = _distance(position,(*IteradorObstaculos)->position);    

        if(dist == 0 || dist < (defensas->radio+(*IteradorObstaculos)->radio)){
            factible = false;
        }    

        IteradorObstaculos++;
    }

    List<Defense*>::iterator IteradorDefenses = defenses.begin();
    dist = 0;

    while(IteradorDefenses != defenses.end() && factible == true){
        dist = _distance(position,(*IteradorDefenses)->position);    

        if(dist == 0 || dist < (defensas->radio+(*IteradorDefenses)->radio)){
            factible = false;
        }    

        IteradorDefenses++;
    }

    return factible;
}

#pragma endregion

#pragma region PlaceDefenses

void placeDefensesSinPreOrden(bool** freeCells, int nCellsWidth, int nCellsHeight, float mapWidth, float mapHeight, std::list<Object*> obstacles, std::list<Defense*> defenses){
    float cellWidth = mapWidth / nCellsWidth;
    float cellHeight = mapHeight / nCellsHeight;     
    float** cellValues = new float*[nCellsHeight];
    
    FreeCell(freeCells,nCellsHeight,nCellsWidth);

    for(int i=0; i<nCellsHeight; ++i){
        cellValues[i] = new float[nCellsWidth];
        for(int j=0; j<nCellsWidth; ++j){
            cellValues[i][j] = defaultCellValue(i,j,freeCells,nCellsWidth,nCellsHeight,mapWidth,mapHeight,obstacles,defenses);
        }
    }

    Vector3 auxiliar;
    auto iterator = defenses.begin();

    while(iterator != defenses.end()){
        auxiliar = Seleccion(freeCells,cellValues,nCellsWidth,nCellsHeight,mapWidth,mapHeight,cellWidth,cellHeight);

        if(Factibilidad(obstacles,defenses,(*iterator),auxiliar,mapWidth,mapHeight)){
            (*iterator)->position = auxiliar;
            iterator++;
        }
    }
}

#pragma endregion

#pragma endregion
//////////////////////////////////




//////////////////////////////////
#pragma region PreordenFusion

#pragma region SeleccionFusion

Vector3 SeleccionFusion(std::vector<CellsMap>& vector, float cellWidth, float cellHeight){
    CellsMap auxiliar=vector.back();

    vector.pop_back();

    return cellCenterToPosition(auxiliar.PositionI,auxiliar.PositionJ,cellWidth,cellHeight);
}

#pragma endregion

#pragma region Fusion

void Fusion(std::vector<CellsMap>& cell,std::vector<CellsMap>& CellsFirst,std::vector<CellsMap>& CellsSecond){
    int sizeFirst = CellsFirst.size();
    int sizeSecond = CellsSecond.size();
    int cel,firstIndex,secondIndex;
    cel = firstIndex = secondIndex = 0;

    while(firstIndex < sizeFirst && secondIndex < sizeSecond){
        if(CellsFirst[firstIndex].value < CellsSecond[secondIndex].value){
            cell[cel] = CellsFirst[firstIndex];
            firstIndex++;
        }else{
            cell[cel] = CellsSecond[secondIndex];
            secondIndex++;
        }
        cel++;
    }

    for(firstIndex; firstIndex<sizeFirst; firstIndex++){
        cell[cel] = CellsFirst[firstIndex];
        cel++;
    }
    
    for(secondIndex; secondIndex<sizeSecond; secondIndex++){
        cell[cel] = CellsSecond[secondIndex];
        cel++;
    }
}

#pragma endregion

#pragma region OrdenFusion

void OrdenacionFusion(std::vector<CellsMap>& cell){
    if(cell.size() <= 1){
        return;
    }
    
    int MID = cell.size()/2;

    std::vector<CellsMap> CellsFirst;
    std::vector<CellsMap> CellsSecond;

    CellsFirst.reserve(MID);
    CellsSecond.reserve(cell.size()-MID);

    std::move(cell.begin(),cell.begin()+MID,std::back_inserter(CellsFirst));
    std::move(cell.begin()+MID,cell.end(),std::back_inserter(CellsSecond));

    OrdenacionFusion(CellsFirst);
    OrdenacionFusion(CellsSecond);
    Fusion(cell,CellsFirst,CellsSecond);
}

#pragma endregion 

#pragma region PlaceDefenses

void placeDefensesFusion(bool** freeCells, int nCellsWidth, int nCellsHeight, float mapWidth, float mapHeight, std::list<Object*> obstacles, std::list<Defense*> defenses){
    float cellWidth = mapWidth / nCellsWidth;
    float cellHeight = mapHeight / nCellsHeight;
    std::vector<CellsMap> Cells;

    for(int i=0; i<nCellsHeight; ++i){
        for(int j=0; j<nCellsWidth; ++j){                        
            Cells.push_back(CellsMap(defaultCellValue(i,j,freeCells,nCellsWidth,nCellsHeight,mapWidth,mapHeight,obstacles,defenses),i,j));
        }
    }

    OrdenacionFusion(Cells);

    Vector3 auxiliar;
    auto iterator = defenses.begin();

    while(iterator != defenses.end()){
        
        auxiliar = SeleccionFusion(Cells,cellWidth,cellHeight);

        if(Factibilidad(obstacles,defenses,(*iterator),auxiliar,mapWidth,mapHeight)){
            (*iterator)->position = auxiliar;
            iterator++;
        }
    }
}

#pragma endregion

#pragma endregion
//////////////////////////////////




//////////////////////////////////
#pragma region PreordenRapidez

#pragma region Rapidez

int pivote(std::vector<CellsMap>& cell, int INICIO, int FIN){
    int pivote = cell[INICIO].value;
    int count = 0;

    for(int iterador = INICIO+1; iterador <= FIN; ++iterador){
        if(cell[iterador].value <= pivote){
            count++;
        }
    }

    int pivoteIndex = INICIO+count;
    std::swap(cell[pivoteIndex],(cell[INICIO]));

    int i = INICIO;
    int j = FIN;

    while(i < pivoteIndex && j > pivoteIndex){
        while(cell[i].value <= pivote){
            i++;
        }
        while(cell[j].value > pivote){
            j--;
        }
        if(i < pivoteIndex && j > pivoteIndex){
            std::swap(cell[i++],cell[j--]);
        }
    }

    return pivoteIndex;
}

#pragma endregion

#pragma region OrdenRapida

void OrdenacionRapida(std::vector<CellsMap>& cell,int INICIO,int FIN){
    if(INICIO > FIN){
        return;
    }

    int MID = pivote(cell,INICIO,FIN);

    OrdenacionRapida(cell,INICIO,MID-1);
    OrdenacionRapida(cell,MID+1,FIN);
}

#pragma endregion

#pragma region SeleccionRapidez

Vector3 SeleccionRapidez(std::vector<CellsMap>& vector, float cellWidth, float cellHeight){
    CellsMap auxiliar=vector.back();

    vector.pop_back();

    return cellCenterToPosition(auxiliar.PositionI,auxiliar.PositionJ,cellWidth,cellHeight);
}

#pragma endregion

#pragma region PlaceDefenses

void placeDefensesRapidez(bool** freeCells, int nCellsWidth, int nCellsHeight, float mapWidth, float mapHeight, std::list<Object*> obstacles, std::list<Defense*> defenses){
    float cellWidth = mapWidth / nCellsWidth;
    float cellHeight = mapHeight / nCellsHeight;   
    std::vector<CellsMap> Cells;

    for(int i=0; i<nCellsHeight; ++i){
        for(int j=0; j<nCellsWidth; ++j){
            Cells.push_back(CellsMap(defaultCellValue(i,j,freeCells,nCellsWidth,nCellsHeight,mapWidth,mapHeight,obstacles,defenses),i,j));
        }
    }

    OrdenacionRapida(Cells,0,Cells.size());

    Vector3 auxiliar;
    auto iterator = defenses.begin();

    while(iterator != defenses.end()){
        auxiliar = SeleccionRapidez(Cells,cellWidth,cellHeight);

        if(Factibilidad(obstacles,defenses,(*iterator),auxiliar,mapWidth,mapHeight)){
            (*iterator)->position = auxiliar;
            iterator++;
        }
    }
}

#pragma endregion

#pragma endregion
//////////////////////////////////




//////////////////////////////////
#pragma region PreordenMonticulo

//FIN
#pragma region SeleccionMonticulo

Vector3 SeleccionMonticulo(std::vector<CellsMap>& vector, float cellWidth, float cellHeight){
    CellsMap auxiliar=vector.back();

    pop_heap(vector.begin(),vector.end()),
    vector.pop_back();

    return cellCenterToPosition(auxiliar.PositionI,auxiliar.PositionJ,cellWidth,cellHeight);
}

#pragma endregion

//FIN
#pragma region OrdenMonticulo

void OrdenacionMonticulo(std::vector<CellsMap>& cell){
    std::make_heap(cell.begin(),cell.end());

    std::sort_heap(cell.begin(),cell.end());
}

#pragma endregion

//FIN
#pragma region PlaceDefenses

void placeDefensesMonticulo(bool** freeCells, int nCellsWidth, int nCellsHeight, float mapWidth, float mapHeight, std::list<Object*> obstacles, std::list<Defense*> defenses){
    float cellWidth = mapWidth / nCellsWidth;
    float cellHeight = mapHeight / nCellsHeight;   
    std::vector<CellsMap> Cells;

    for(int i=0; i<nCellsHeight; ++i){
        for(int j=0; j<nCellsWidth; ++j){
            Cells.push_back(CellsMap(defaultCellValue(i,j,freeCells,nCellsWidth,nCellsHeight,mapWidth,mapHeight,obstacles,defenses),i,j));
        }
    }

    OrdenacionMonticulo(Cells);

    Vector3 auxiliar;
    auto iterator = defenses.begin();

    while(iterator != defenses.end()){
        auxiliar = SeleccionMonticulo(Cells,cellWidth,cellHeight);

        if(Factibilidad(obstacles,defenses,(*iterator),auxiliar,mapWidth,mapHeight)){
            (*iterator)->position = auxiliar;
            iterator++;
        }
    }
}

#pragma endregion

#pragma endregion
//////////////////////////////////


void DEF_LIB_EXPORTED placeDefenses3(bool** freeCells, int nCellsWidth, int nCellsHeight, float mapWidth, float mapHeight, List<Object*> obstacles, List<Defense*> defenses){
    float cellWidth = mapWidth / nCellsWidth;
    float cellHeight = mapHeight / nCellsHeight; 

	cronometro CronoSinPreOrden;
    long int rSPO = 0;
    CronoSinPreOrden.activar();
    do{	
		placeDefensesSinPreOrden(freeCells,nCellsWidth,nCellsHeight,mapWidth,mapHeight,obstacles,defenses);
		++rSPO;
    }while(CronoSinPreOrden.tiempo() < 0.01/0.1+0.01);
    CronoSinPreOrden.parar();
    
    //FUSION
    cronometro CronoFusion;
    long int rF = 0;
    CronoFusion.activar();
    do{	
		placeDefensesFusion(freeCells,nCellsWidth,nCellsHeight,mapWidth,mapHeight,obstacles,defenses);
		++rF;
    }while(CronoFusion.tiempo() < 0.01/0.1+0.01);
    CronoFusion.parar();

    //RAPIDEZ
    cronometro CronoRapidez;
    long int rR = 0;
    CronoRapidez.activar();
    do{	
		placeDefensesRapidez(freeCells,nCellsWidth,nCellsHeight,mapWidth,mapHeight,obstacles,defenses);
		++rR;
    }while(CronoRapidez.tiempo() < 0.01/0.1+0.01);
    CronoRapidez.parar();

    //MONTICULO
    cronometro CronoMonticulo;
    long int rM = 0;
    CronoMonticulo.activar();
    do{	
		placeDefensesMonticulo(freeCells,nCellsWidth,nCellsHeight,mapWidth,mapHeight,obstacles,defenses);
		++rM;
    }while(CronoMonticulo.tiempo() < 0.01/0.1+0.01);
    CronoMonticulo.parar();


    std::cout<<(nCellsWidth * nCellsHeight)<<'\t'<<CronoSinPreOrden.tiempo()/rSPO<<'\t'<<CronoFusion.tiempo()/rF<<'\t'<<CronoRapidez.tiempo()/rR<<'\t'<<CronoMonticulo.tiempo()/rM<<std::endl;
}