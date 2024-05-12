#include "raylib.h"
#include "raymath.h"
#include <stdlib.h>

struct sblockmodel
{
		Model model;
		char isload;
}BlockModel;
Model GetBlockModel()
{
	if(BlockModel.isload==0)
	{
		Mesh blockmesh = GenMeshCube(1,1,1);
		BlockModel.model = LoadModelFromMesh(blockmesh);
		BlockModel.isload = 1;
	}
	return BlockModel.model;
}
void DrawBlockModel(int x,int y,int z,Color col)
{
	const Color colwire = ColorAlpha(BLACK,0.2f);
	const Vector3 vec3 = (Vector3){
			(float)x,
			(float)y,
			(float)z};
	DrawModel(GetBlockModel(),vec3,1,col);
	DrawCubeWiresV(vec3,Vector3One(),colwire);
}
void FreeBlockModel()
{
	UnloadModel(BlockModel.model);
}
//Material MaterialColor(Color col)
//{
	//Material mat = LoadMaterialDefault();
	//Image img = GenImageColor(1,1,col);
	//Texture t = LoadTextureFromImage(img);
	//SetMaterialTexture(&mat,MATERIAL_MAP_DIFFUSE,t);
	//UnloadImage(img);
	////UnloadTexture(t);
	//return mat;
//}
#define BLOCKSIZEMAX 16
#define BLOCKMAPMAX BLOCKSIZEMAX*BLOCKSIZEMAX*BLOCKSIZEMAX

struct sblockentity
{
	int w;
	int h;
	int d;
	Color colors[32];
	char map[BLOCKMAPMAX];
	int x;
	int y;
	int z;
};
typedef struct sblockentity BlockEntity;
BlockEntity BlockInit(int w,int h,int d)
{	
	BlockEntity sbc={w,h,d,
		{GRAY,RED,GREEN,BROWN},
		{0}
	};
	
	return sbc;
}

void BlockSet(BlockEntity* be,int px,int py,int pz,char idcol)
{
	int id=0;
	if(px<be->w && py<be->h && pz<be->d)
	{
		for(int z=0;z<be->d;z++)
		for(int y=0;y<be->h;y++)
		for(int x=0;x<be->w;x++)
		{
			if(z==pz && y==py && z==px)
			{
				be->map[id]=idcol;
				return;
			}
			id++;
		}
	}
}

void BlockDraw(struct sblockentity sblk)
{

	const int W2 = sblk.w/2;
	const int H2 = sblk.h/2;
	const int D2 = sblk.d/2;
	//Matrix mt={0};
	int idmap=0;
	int idcol;
	
	for(int z=-D2;z<D2;z++)
	for(int y=-H2;y<H2;y++)
	for(int x=-W2;x<W2;x++)
	{
		idcol = sblk.map[idmap];
		if(idcol!=-1)
			DrawBlockModel(
			sblk.x+x,
			sblk.y+y,
			sblk.z+z,
			sblk.colors[idcol]
			);
		idmap++;
	}
}

int main(void)
{

    // Initialization
    //--------------------------------------------------------------------------------------
   // float fraq = GetMonitorWidth(0)/GetMonitorHeight(0);
    const int screenWidth = 1000; //GetMonitorWidth(0);
    const int screenHeight = 600;//GetMonitorHeight(0);
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");


    SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
    //Font f = LoadFont(".ttf");

	Camera cam ={
        (Vector3){ 20.0f, 5.0f, 18.0f }, // Camera position
        (Vector3){ 0.0f, 0.0f, -30.0f },      // Camera looking at point
        (Vector3){ 0.0f, 1.0f, 0.0f },          // Camera up vector (rotation towards target)
        45.0f,                                // Camera field-of-view Y
        CAMERA_PERSPECTIVE
    };
	BlockEntity be = BlockInit(16,16,16);
	BlockEntity be2 = BlockInit(2,2,2);
	BlockSet(&be2,1,1,1,-1);
	be2.x = 5;
	be2.z = 5;
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        //if(IsKeyDown(KEY_S))
       
        BeginDrawing();
        ClearBackground(BLUE);
        
		BeginMode3D(cam);
		BlockDraw(be);
		BlockDraw(be2);
		
		EndMode3D();
		
        EndDrawing();
    }
   // UnloadModel(model);
   //UnloadModel(mo);
	FreeBlockModel();
    CloseWindow();        // Close window and OpenGL
    return 0;
}

