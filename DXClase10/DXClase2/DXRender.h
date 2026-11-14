#ifndef _DXRender_
#define _DXRender_

#include "Constants.h"

#include "Camara.h"
#include "Light.h"
#include "MeshPrimitives.h"

class DXRender
{
private:
	//Global Declarations//
	IDXGISwapChain* SwapChain;
	ID3D11Device* d3d11Device;
	ID3D11DeviceContext* d3d11DevCon;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11DepthStencilView* depthStencilView;

	IDirectInputDevice8* DIKeyboard;
	IDirectInputDevice8* DIMouse;
	DIMOUSESTATE mouseLastState;
	LPDIRECTINPUT8 DirectInput;

	D3DXMATRIX View;
	D3DXMATRIX Projection;


	MPrimitives* cubo;
	MPrimitives* esfera;
	MPrimitives* toroide;
	MPrimitives* cono;
	MPrimitives* cilindro;
	MPrimitives* billboard;
	MPrimitives* iconoTomar;
	//FUEGOOO
	MPrimitives* fuego[3];
	struct Fuego
	{
		D3DXVECTOR3 pos;
		float tam;
		bool activo;
	};
	Fuego fuegos[10];
	int numFuegos;
	//
	MPrimitives* skydomo;
	MPrimitives* terreno;
	MPrimitives* Bitmap;
	// RECOGER ITEMS - datos de cada itm
	struct Item
	{
		MPrimitives* modelo;
		MPrimitives* icono;
		D3DXVECTOR3 pos;
		float escala;
		bool recogido;
	};
	Item items[1];
	static const int CUBETA = 0;
	bool teclaFAntes;
	int numItems;
	int itemCercano;
	int itemEnMano;
	bool teclaEAntes;
	MPrimitives* BoundingSphere1;
	MPrimitives* BoundingSphere2;

	//HUD
	MPrimitives* hudVida;
	MPrimitives* hudMapa;

	//Terreno modelos Lab 440
	MPrimitives* paredesArriba;
	MPrimitives* marcosArriba;
	MPrimitives* paredesArribaVentanas;
	MPrimitives* letreroLet1;
	MPrimitives* letreroLet3;
	MPrimitives* letreroLet4;
	MPrimitives* letreroLet5;
	MPrimitives* letreroLet6;
	MPrimitives* letreroLet7;
	MPrimitives* letreroLet8;
	MPrimitives* letreroLet9;
	MPrimitives* letreroLet10;
	MPrimitives* letreroLet11;
	MPrimitives* franja;
	MPrimitives* paredesAbajo;
	MPrimitives* paredesAbajoPB;
	MPrimitives* piso;
	MPrimitives* rejasMetal;
	MPrimitives* rejasFranja;
	MPrimitives* rejasTierra;
	MPrimitives* rejasTecho;
	MPrimitives* banquetaPiso;
	MPrimitives* banquetaPiso2;
	MPrimitives* ventanas;
	MPrimitives* invernaderoVentana;
	MPrimitives* invernaderoVidrio;
	MPrimitives* marcosMarco;
	MPrimitives* marcosPB;
	MPrimitives* marcosPuerta;
	MPrimitives* estructInvernadero;
	MPrimitives* pisoTierra;
	MPrimitives* techoPrincipal;
	MPrimitives* techoTierra;
	MPrimitives* techoOrillaMarco;
	MPrimitives* techoOrillaPB;
	MPrimitives* puertasPuerta;
	MPrimitives* puertasMarco;
	MPrimitives* puertasPB;
	MPrimitives* marcosVentanas;
	MPrimitives* celdaMetal;
	MPrimitives* celdaVidrio;
	MPrimitives* celdaLuces;
	// Partes de los paneles
	MPrimitives* panelMetal;
	MPrimitives* panelPared;
	MPrimitives* panelPantalla;

	// Botones de diferentes colores
	MPrimitives* panelBotonRojo;
	MPrimitives* panelBotonVerde;
	MPrimitives* panelBotonAzul;

	// Posiciones de los tres paneles
	D3DXMATRIX matrizPanel1;
	D3DXMATRIX matrizPanel2;
	D3DXMATRIX matrizPanel3;

	MPrimitives* escritorioDesk;
	MPrimitives* escritorioDeskOld;
	MPrimitives* escritorioKeyboardOld;
	MPrimitives* escritorioKeybroard;
	MPrimitives* escritorioGrid;
	MPrimitives* escritorioGridOld;
	MPrimitives* escritorioCables;
	MPrimitives* escritorioCablesOld;
	MPrimitives* escritorioScreen;
	MPrimitives* escritorioScreenOld;
	MPrimitives* escritorioSoloScreen;
	MPrimitives* escritorioSoloScreenOld;

	D3DXMATRIX matrizEscritorio;

	MPrimitives* compus;
	D3DXMATRIX matrizCompus;





	Camara* camara;
	Camara* skyCamara;

	float rotPerFrame;
	float sphereSpeed;
	D3DXVECTOR2 BoundingSphereTranslation;
	D3DXVECTOR2 DummyBoundyEvents;
	D3DXVECTOR2 LastBoundingPosition;
	// Matrices para las tres celdas criogenicas
	D3DXMATRIX matrizCelda1;
	D3DXMATRIX matrizCelda2;
	D3DXMATRIX matrizCelda3;


public:
	DXRender()
	{
		rotPerFrame = 0.0f;
	}

	// RECOGER ITEMS -fn para crear un item y recibe modelo

	Item CrearItem(MPrimitives* modelo, LPCWSTR icono, D3DXVECTOR3 pos, float escala)
	{
		Item it;
		it.modelo = modelo;
		it.icono = new MPrimitives(d3d11Device, d3d11DevCon, 1, 1, icono, camara);
		it.pos = pos;
		it.escala = escala;
		it.recogido = false;
		return it;
	}

	// Fuegos

	void AgregarFuego(float x, float z, float tam)
	{
		fuegos[numFuegos].pos = D3DXVECTOR3(x, 1.18f + 2.0f * tam, z);
		fuegos[numFuegos].tam = tam;
		fuegos[numFuegos].activo = true;
		numFuegos++;
	}

	bool InitDevice(HWND hwnd, int Width, int Height)
	{
		//Describe our Buffer
		DXGI_MODE_DESC bufferDesc;

		ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

		bufferDesc.Width = Width;
		bufferDesc.Height = Height;
		bufferDesc.RefreshRate.Numerator = 60;
		bufferDesc.RefreshRate.Denominator = 1;
		bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		//Describe our SwapChain
		DXGI_SWAP_CHAIN_DESC swapChainDesc;

		ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

		swapChainDesc.BufferDesc = bufferDesc;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1;
		swapChainDesc.OutputWindow = hwnd;
		swapChainDesc.Windowed = TRUE;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		//Create our SwapChain
		D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,
			D3D11_SDK_VERSION, &swapChainDesc, &SwapChain, &d3d11Device, NULL, &d3d11DevCon);

		//Create our BackBuffer
		ID3D11Texture2D* BackBuffer;
		SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);

		//Create our Render Target
		d3d11Device->CreateRenderTargetView(BackBuffer, NULL, &renderTargetView);
		BackBuffer->Release();

		//Crea la descripcion del raster, el cual determina como se dibujaran los poligonos
		D3D11_RASTERIZER_DESC rasterDesc;
		ID3D11RasterizerState* rasterState;
		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_NONE;//se puede manipular para transparencias, cambiar a D3D11_CULL_NONE;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0;

		d3d11Device->CreateRasterizerState(&rasterDesc, &rasterState);
		d3d11DevCon->RSSetState(rasterState);

		//Describe our Depth/Stencil Buffer
		D3D11_TEXTURE2D_DESC depthStencilDesc;
		ID3D11Texture2D* depthStencilBuffer;

		depthStencilDesc.Width = Width;
		depthStencilDesc.Height = Height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		//Create the Depth/Stencil View
		d3d11Device->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);
		d3d11Device->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);

		//Set our Render Target
		d3d11DevCon->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

		cubo = new MPrimitives(d3d11Device, d3d11DevCon, 2.0f, L"Texturas/escudo.jpg", L"Texturas/escudoNormal.jpg", ShaderDiffuse);
		esfera = new MPrimitives(d3d11Device, d3d11DevCon, 32, 32, 1.0f, L"Texturas/escudo.jpg", L"Texturas/escudoNormal.jpg", ShaderDiffuse);
		toroide = new MPrimitives(d3d11Device, d3d11DevCon, 32, 32, 1.0f, 0.5f, L"Texturas/escudo.jpg", L"Texturas/escudoNormal.jpg", ShaderDiffuse);
		cilindro = new MPrimitives(d3d11Device, d3d11DevCon, 16, 1.0f, 1.0f, 2.0f, L"Texturas/escudo.jpg", L"Texturas/escudoNormal.jpg", ShaderDiffuse);
		cono = new MPrimitives(d3d11Device, d3d11DevCon, 16, 0.0f, 1.0f, 2.0f, L"Texturas/escudo.jpg", L"Texturas/escudoNormal.jpg", ShaderDiffuse);
		billboard = new MPrimitives(d3d11Device, d3d11DevCon, 2.0f, 2.0f, L"Texturas/ave.png");
		// RECOGER ITEMS icono
		iconoTomar = new MPrimitives(d3d11Device, d3d11DevCon, 1.0f, 1.0f, L"Texturas/ave.png");

		//Fuego
		fuego[0] = new MPrimitives(d3d11Device, d3d11DevCon, 4.0f, 3.0f, L"Texturas/ave.png", ShaderGUI);
		fuego[1] = new MPrimitives(d3d11Device, d3d11DevCon, 4.0f, 3.0f, L"Texturas/ave.png", ShaderGUI);
		fuego[2] = new MPrimitives(d3d11Device, d3d11DevCon, 4.0f, 3.0f, L"Texturas/ave.png", ShaderGUI);
		numFuegos = 0;
		AgregarFuego(157.5f, 130.0f, 1.0f);
		AgregarFuego(130.0f, 152.0f, 1.3f);
		AgregarFuego(185.0f, 170.0f, 0.8f);
		AgregarFuego(125.0f, 125.0f, 0.7f);

		skydomo = new MPrimitives(d3d11Device, d3d11DevCon, 32, 32, 1000.0f, L"Texturas/sky2.jpg", L"Texturas/escudoNormal.jpg", ShaderDiffuse);
		terreno = new MPrimitives(d3d11Device, d3d11DevCon, 100.0f, L"Texturas/leaves.jpg", L"Texturas/tierra.png", "Texturas/height3.bmp", ShaderMultitextura);

		// Estructura laboratorio
		// PAREDES SUPERIORES ACTUALIZADAS
		paredesArriba = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/Pared_Arriba/paredes_Arriba_paredes.txt", L"../Objetos/plantilla/DXclase1/Pared_Arriba/pared2.png", L"", ShaderDiffuse);
		marcosArriba = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/Pared_Arriba/paredes_Arriba_marcos.txt", L"../Objetos/plantilla/DXclase1/Pared_Arriba/marco.png", L"", ShaderDiffuse);
		paredesArribaVentanas = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/Pared_Arriba/paredes_Arriba_ventanas.txt", L"../Objetos/plantilla/DXclase1/Pared_Arriba/ventana2.jpeg", L"", ShaderDiffuse);

		// LETREROS NUEVOS
		letreroLet1 = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/Pared_Arriba/paredes_Arriba_let1.txt", L"../Objetos/plantilla/DXclase1/Pared_Arriba/let1.png", L"", ShaderDiffuse);
		letreroLet3 = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/Pared_Arriba/paredes_Arriba_let3_.txt", L"../Objetos/plantilla/DXclase1/Pared_Arriba/let3}.png", L"", ShaderDiffuse);
		letreroLet4 = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/Pared_Arriba/paredes_Arriba_let4.txt", L"../Objetos/plantilla/DXclase1/Pared_Arriba/let4.png", L"", ShaderDiffuse);
		letreroLet5 = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/Pared_Arriba/paredes_Arriba_let5.txt", L"../Objetos/plantilla/DXclase1/Pared_Arriba/let5.png", L"", ShaderDiffuse);
		letreroLet6 = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/Pared_Arriba/paredes_Arriba_let6.txt", L"../Objetos/plantilla/DXclase1/Pared_Arriba/let6.png", L"", ShaderDiffuse);
		letreroLet7 = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/Pared_Arriba/paredes_Arriba_let7.txt", L"../Objetos/plantilla/DXclase1/Pared_Arriba/let7.png", L"", ShaderDiffuse);
		letreroLet8 = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/Pared_Arriba/paredes_Arriba_let8.txt", L"../Objetos/plantilla/DXclase1/Pared_Arriba/let8.png", L"", ShaderDiffuse);
		letreroLet9 = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/Pared_Arriba/paredes_Arriba_let9.txt", L"../Objetos/plantilla/DXclase1/Pared_Arriba/let9.png", L"", ShaderDiffuse);
		letreroLet10 = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/Pared_Arriba/paredes_Arriba_let10.txt", L"../Objetos/plantilla/DXclase1/Pared_Arriba/let10.png", L"", ShaderDiffuse);
		letreroLet11 = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/Pared_Arriba/paredes_Arriba_let11.txt", L"../Objetos/plantilla/DXclase1/Pared_Arriba/let11.png", L"", ShaderDiffuse);

		franja = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/Franja/franja.txt", L"../Objetos/plantilla/DXclase1/Franja/franja2.png", L"", ShaderDiffuse);
		paredesAbajo = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/pared_Abajo/pared_Abajo_paredes.txt", L"../Objetos/plantilla/DXclase1/pared_Abajo/pared2.png", L"", ShaderDiffuse);
		paredesAbajoPB = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/pared_Abajo/pared_Abajo_pb.txt", L"../Objetos/plantilla/DXclase1/pared_Abajo/padredpb.png", L"", ShaderDiffuse);
		piso = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/piso/piso.txt", L"../Objetos/plantilla/DXclase1/piso/piso.png", L"", ShaderDiffuse);
		rejasMetal = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/rejas/rejas_metal.txt", L"../Objetos/plantilla/DXclase1/rejas/metal2.png", L"", ShaderDiffuse);
		rejasFranja = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/rejas/rejas_franja.txt", L"../Objetos/plantilla/DXclase1/rejas/franja2.png", L"", ShaderDiffuse);
		rejasTierra = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/rejas/rejas_tierra.txt", L"../Objetos/plantilla/DXclase1/rejas/tierra.png", L"", ShaderDiffuse);
		rejasTecho = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/rejas/rejas_techo.txt", L"../Objetos/plantilla/DXclase1/rejas/techo.png", L"", ShaderDiffuse);
		banquetaPiso = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/banqueta/banqueta_piso.txt", L"../Objetos/plantilla/DXclase1/banqueta/piso.png", L"", ShaderDiffuse);
		banquetaPiso2 = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/banqueta/banqueta_piso2.txt", L"../Objetos/plantilla/DXclase1/banqueta/piso2.png", L"", ShaderDiffuse);
		ventanas = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/ventanas/ventanas.txt", L"../Objetos/plantilla/DXclase1/ventanas/ventana2.jpeg", L"", ShaderDiffuse);
		invernaderoVentana = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/invernadero_techo/techo_invernadero_ventana.txt", L"../Objetos/plantilla/DXclase1/invernadero_techo/ventana2.jpeg", L"", ShaderDiffuse);
		invernaderoVidrio = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/invernadero_techo/techo_invernadero_vidrio.txt", L"../Objetos/plantilla/DXclase1/invernadero_techo/vidrio.png", L"", ShaderDiffuse);
		marcosMarco = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/marcos/marcos_marco.txt", L"../Objetos/plantilla/DXclase1/marcos/marco.png", L"", ShaderDiffuse);
		marcosPuerta = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/marcos/marcos_puerta.txt", L"../Objetos/plantilla/DXclase1/marcos/puerta2.png", L"", ShaderDiffuse);
		marcosPB = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/marcos/marcos_pb.txt", L"../Objetos/plantilla/DXclase1/marcos/padredpb.png", L"", ShaderDiffuse);
		estructInvernadero = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/estruct_invernadero/estruct_invernadero.txt", L"../Objetos/plantilla/DXclase1/estruct_invernadero/marco.png", L"", ShaderDiffuse);
		pisoTierra = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/piso_tierra/piso_tierra.txt", L"../Objetos/plantilla/DXclase1/piso_tierra/tierra.png", L"", ShaderDiffuse);
		techoPrincipal = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/techo/techo_techo.txt", L"../Objetos/plantilla/DXclase1/techo/techo.png", L"", ShaderDiffuse);
		techoTierra = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/techo/techo_tierra.txt", L"../Objetos/plantilla/DXclase1/techo/tierra.png", L"", ShaderDiffuse);
		techoOrillaMarco = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/techo_orilla/techo_orilla_marco.txt", L"../Objetos/plantilla/DXclase1/techo_orilla/marco.png", L"", ShaderDiffuse);
		techoOrillaPB = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/techo_orilla/techo_orilla_pb.txt", L"../Objetos/plantilla/DXclase1/techo_orilla/padredpb.png", L"", ShaderDiffuse);
		puertasPuerta = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/puertas/puertas_puerta.txt", L"../Objetos/plantilla/DXclase1/puertas/puerta2.png", L"", ShaderDiffuse);
		puertasMarco = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/puertas/puertas_marco.txt", L"../Objetos/plantilla/DXclase1/puertas/marco.png", L"", ShaderDiffuse);
		puertasPB = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/puertas/puertas_pb.txt", L"../Objetos/plantilla/DXclase1/puertas/padredpb.png", L"", ShaderDiffuse);
		marcosVentanas = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/marco_ventana/marco_ventanas.txt", L"../Objetos/plantilla/DXclase1/marco_ventana/marco.png", L"", ShaderDiffuse);
		celdaMetal = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/celdas/celda_metal.txt", L"../Objetos/plantilla/DXclase1/celdas/metal.png", L"", ShaderDiffuse);
		celdaVidrio = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/celdas/celda_vidrio.txt", L"../Objetos/plantilla/DXclase1/celdas/vidrio.png", L"", ShaderDiffuse);
		celdaLuces = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/celdas/celda_luces.txt", L"../Objetos/plantilla/DXclase1/celdas/luz.png", L"", ShaderDiffuse);
		panelMetal = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/paneles/panel_metal.txt", L"../Objetos/plantilla/DXclase1/paneles/metal.png", L"", ShaderDiffuse);
		panelPared = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/paneles/panel_pared.txt", L"../Objetos/plantilla/DXclase1/paneles/pared2.png", L"", ShaderDiffuse);
		panelPantalla = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/paneles/panel_pantalla.txt", L"../Objetos/plantilla/DXclase1/paneles/pantalla.png", L"", ShaderDiffuse);
		panelBotonRojo = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/paneles/panel_boton.txt", L"../Objetos/plantilla/DXclase1/paneles/rojo.jpg", L"", ShaderDiffuse);
		panelBotonVerde = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/paneles/panel_boton.txt", L"../Objetos/plantilla/DXclase1/paneles/verde.jpg", L"", ShaderDiffuse);
		panelBotonAzul = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/paneles/panel_boton.txt", L"../Objetos/plantilla/DXclase1/paneles/azul.jpg", L"", ShaderDiffuse);
		escritorioDesk = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/escritorio/escritorio_desk.txt", L"../Objetos/plantilla/DXclase1/escritorio/desk_BaseColor.png", L"", ShaderDiffuse);
		escritorioDeskOld = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/escritorio/escritorio_deskold.txt", L"../Objetos/plantilla/DXclase1/escritorio/desk_old_BaseColor.png", L"", ShaderDiffuse);
		escritorioKeyboardOld = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/escritorio/escritorio_keyboardold.txt", L"../Objetos/plantilla/DXclase1/escritorio/keyboard_old_BaseColor.png", L"", ShaderDiffuse);
		escritorioKeybroard = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/escritorio/escritorio_keybroard.txt", L"../Objetos/plantilla/DXclase1/escritorio/keybroard_BaseColor.png", L"", ShaderDiffuse);
		escritorioGrid = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/escritorio/escritorio_grid.txt", L"../Objetos/plantilla/DXclase1/escritorio/grid_BaseColor.png", L"", ShaderDiffuse);
		escritorioGridOld = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/escritorio/escritorio_gridoldold.txt", L"../Objetos/plantilla/DXclase1/escritorio/grid_old_BaseColor.png", L"", ShaderDiffuse);
		escritorioCables = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/escritorio/escritorio_cables.txt", L"../Objetos/plantilla/DXclase1/escritorio/cables_BaseColor.png", L"", ShaderDiffuse);
		escritorioCablesOld = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/escritorio/escritorio_cablesold.txt", L"../Objetos/plantilla/DXclase1/escritorio/cables_old_BaseColor.png", L"", ShaderDiffuse);
		escritorioScreen = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/escritorio/escritorio_screen.txt", L"../Objetos/plantilla/DXclase1/escritorio/screen_BaseColor.png", L"", ShaderDiffuse);
		escritorioScreenOld = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/escritorio/escritorio_screenold.txt", L"../Objetos/plantilla/DXclase1/escritorio/screen_old_BaseColor.png", L"", ShaderDiffuse);
		escritorioSoloScreen = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/escritorio/escritorio_soloscreen.txt", L"../Objetos/plantilla/DXclase1/escritorio/soloscreen_BaseColor.png", L"", ShaderDiffuse);
		escritorioSoloScreenOld = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/escritorio/escritorio_soloscreenold.txt", L"../Objetos/plantilla/DXclase1/escritorio/soloscreen_old_BaseColor.png", L"", ShaderDiffuse);
		compus = new MPrimitives(d3d11Device, d3d11DevCon, (char*)"../Objetos/plantilla/DXclase1/computadoras/compus_Trim_sheet.txt", L"../Objetos/plantilla/DXclase1/computadoras/Trim_sheet_BaseColor.png", L"", ShaderDiffuse);








		BoundingSphere1 = new MPrimitives(d3d11Device, d3d11DevCon, 32, 32, 1.0f, L"Texturas/escudo.jpg", L"Texturas/escudoNormal.jpg", ShaderDiffuse);
		BoundingSphere2 = new MPrimitives(d3d11Device, d3d11DevCon, 32, 32, 1.0f, L"Texturas/escudo.jpg", L"Texturas/escudoNormal.jpg", ShaderDiffuse);
		BoundingSphereTranslation = D3DXVECTOR2(0, 0);

		return true;
	}

	bool InitDirectInput(HINSTANCE hInstance, HWND hwnd)
	{
		DirectInput8Create(hInstance,
			DIRECTINPUT_VERSION,
			IID_IDirectInput8,
			(void**)&DirectInput,
			NULL);

		DirectInput->CreateDevice(GUID_SysKeyboard,
			&DIKeyboard,
			NULL);

		DirectInput->CreateDevice(GUID_SysMouse,
			&DIMouse,
			NULL);

		DIKeyboard->SetDataFormat(&c_dfDIKeyboard);
		DIKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

		DIMouse->SetDataFormat(&c_dfDIMouse);
		DIMouse->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);

		return true;
	}

	void DetectInput()
	{
		DIMOUSESTATE mouseCurrState;

		BYTE keyboardState[256];

		DIKeyboard->Acquire();
		DIMouse->Acquire();

		DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);

		DIKeyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);

		// RECOGER ITEMS con E
		bool teclaE = (keyboardState[DIK_E] & 0x80) != 0;
		if (teclaE && !teclaEAntes && itemCercano >= 0)
		{
			items[itemCercano].recogido = true;
			itemEnMano = itemCercano;
		}
		teclaEAntes = teclaE;
		bool teclaF = (keyboardState[DIK_F] & 0x80) != 0;
		if (teclaF && !teclaFAntes && itemEnMano == CUBETA)
		{
			int cercano = -1;
			float distMin = 10.0f;
			for (int i = 0; i < numFuegos; i++)
			{
				if (!fuegos[i].activo) continue;
				D3DXVECTOR3 dif = camara->Position - fuegos[i].pos;
				dif.y = 0;
				float d = D3DXVec3Length(&dif);
				if (d < distMin) { distMin = d; cercano = i; }
			}
			if (cercano >= 0) fuegos[cercano].activo = false;
		}
		teclaFAntes = teclaF;

		float speed = 0.05f;

		float moveLeftRight = 0;
		float moveBackForward = 0;

		float camYaw = 0;
		float camPitch = 0;



		if (keyboardState[DIK_A])
		{
			moveLeftRight -= speed;
		}
		if (keyboardState[DIK_D])
		{
			moveLeftRight += speed;
		}
		if (keyboardState[DIK_W])
		{
			moveBackForward += speed;
		}
		if (keyboardState[DIK_S])
		{
			moveBackForward -= speed;
		}
		if (keyboardState[DIK_LEFT])
		{
			DummyBoundyEvents.x = -.01f;
		}
		if (keyboardState[DIK_RIGHT])
		{
			DummyBoundyEvents.x = .01f;
		}
		if (keyboardState[DIK_UP])
		{
			DummyBoundyEvents.y = .01f;
		}
		if (keyboardState[DIK_DOWN])
		{
			DummyBoundyEvents.y = -.01f;
		}

		if ((mouseCurrState.lX != mouseLastState.lX) || (mouseCurrState.lY != mouseLastState.lY))
		{
			float smoothness = 0.001f;
			camYaw = mouseCurrState.lX * smoothness;

			camPitch = mouseCurrState.lY * smoothness;

			mouseLastState = mouseCurrState;
		}

		camara->UpdateCamera(moveBackForward, moveLeftRight, camYaw, camPitch);
		skyCamara->setSkyCamara(camara);
	}

	void ReleaseObjects()
	{
		//Release the COM Objects we created
		SwapChain->Release();
		d3d11Device->Release();
		d3d11DevCon->Release();
		renderTargetView->Release();
		depthStencilView->Release();
	}

	bool InitScene(int Width, int Height)
	{
		//Create the Viewport
		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = Width;
		viewport.Height = Height;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		//set light values
		light.direction = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		light.ambient = D3DXVECTOR4(0.3f, 0.3f, 0.3f, 1.0f);
		light.diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);

		//Set the Viewport
		d3d11DevCon->RSSetViewports(1, &viewport);

		//Create the camera
		D3DXVECTOR3 Position = D3DXVECTOR3(265.25f, 5.0f, 103.25f);
		D3DXVECTOR3 Target = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);



		camara = new Camara(Position, Target, Up, Width, Height);
		camara->Yaw = D3DXToRadian(270.0f);
		skyCamara = new Camara(Position, Target, Up, Width, Height);
		//esta es la gui
		hudVida = new MPrimitives(d3d11Device, d3d11DevCon, 1, 1, L"Texturas/Health.png", camara);
		hudMapa = new MPrimitives(d3d11Device, d3d11DevCon, 1, 1, L"Texturas/mapa.png", camara);


		// RECOGER ITEMS
		items[CUBETA] = CrearItem(new MPrimitives(d3d11Device, d3d11DevCon, 16, 1.0f, 0.8f, 1.5f, L"Texturas/escudo.jpg", L"Texturas/escudoNormal.jpg", ShaderDiffuse), L"Texturas/Health.png", D3DXVECTOR3(157.5f, 2.2f, 100.0f), 1.0f);
		numItems = 1;
		itemCercano = -1;
		itemEnMano = -1;
		teclaEAntes = false;
		teclaFAntes = false;

		return true;
	}

	void UpdateScene()
	{

		LastBoundingPosition = BoundingSphereTranslation;
		BoundingSphereTranslation += DummyBoundyEvents;


		rotPerFrame += 0.1f;

		D3DXMATRIX rotMatY, northMat, westMat, eastMat, southMat, scale, rotMatX, MatAcomodo;

		D3DXMATRIX Bounding1, Bounding2, BoundinMatTranslation;

		D3DXMatrixRotationY(&rotMatY, D3DXToRadian(rotPerFrame));
		D3DXMatrixTranslation(&MatAcomodo, 50, 3, 50);
		D3DXMatrixTranslation(&northMat, 0, 3, 0);
		D3DXMatrixTranslation(&southMat, 0, -3, 0);
		D3DXMatrixTranslation(&eastMat, 3, 0, 0);
		D3DXMatrixTranslation(&westMat, -3, 0, 0);
		D3DXMatrixTranslation(&Bounding1, 3, 6, 0);
		D3DXMatrixTranslation(&Bounding2, -3, 6, 0);
		D3DXMatrixTranslation(&BoundinMatTranslation, BoundingSphereTranslation.x, BoundingSphereTranslation.y, 0);
		D3DXMatrixScaling(&scale, 0.03f, 0.03f, 0.03f);

		cubo->UpdateScene(rotMatY * MatAcomodo);
		esfera->UpdateScene(rotMatY * northMat * MatAcomodo);
		toroide->UpdateScene(rotMatY * eastMat * MatAcomodo);
		cilindro->UpdateScene(rotMatY * eastMat * northMat * MatAcomodo);
		cono->UpdateScene(rotMatY * westMat * MatAcomodo);

		billboard->UpdateScene(westMat * northMat * MatAcomodo);
		//Laboratorio estructura

	// Laboratorio completo
	// Rotacion de 180 grados en el eje Y

		D3DXMATRIX alOrigen, rotacionLab, posicionLab, matrizLaboratorio;

		// lleva el centro del edificio al origen x=78, z=-94
		D3DXMatrixTranslation(&alOrigen, -78.0f, 0.0f, 94.0f);
		// gira sobre su propio centro
		D3DXMatrixRotationY(&rotacionLab, D3DXToRadian(180.0f));
		// ponerlo en donde quiera (aqui en centro)
		D3DXMatrixTranslation(&posicionLab, 191.25f, 1.0f, 191.25f);

		matrizLaboratorio = alOrigen * rotacionLab * posicionLab;

		// PAREDES SUPERIORES Y LETREROS
		paredesArriba->UpdateScene(matrizLaboratorio);
		marcosArriba->UpdateScene(matrizLaboratorio);
		paredesArribaVentanas->UpdateScene(matrizLaboratorio);

		letreroLet1->UpdateScene(matrizLaboratorio);
		letreroLet3->UpdateScene(matrizLaboratorio);
		letreroLet4->UpdateScene(matrizLaboratorio);
		letreroLet5->UpdateScene(matrizLaboratorio);
		letreroLet6->UpdateScene(matrizLaboratorio);
		letreroLet7->UpdateScene(matrizLaboratorio);
		letreroLet8->UpdateScene(matrizLaboratorio);
		letreroLet9->UpdateScene(matrizLaboratorio);
		letreroLet10->UpdateScene(matrizLaboratorio);
		letreroLet11->UpdateScene(matrizLaboratorio);

		// Franja
		franja->UpdateScene(matrizLaboratorio);

		// Paredes inferiores
		paredesAbajo->UpdateScene(matrizLaboratorio);
		paredesAbajoPB->UpdateScene(matrizLaboratorio);

		// Piso
		piso->UpdateScene(matrizLaboratorio);

		// Rejas
		rejasMetal->UpdateScene(matrizLaboratorio);
		rejasFranja->UpdateScene(matrizLaboratorio);
		rejasTierra->UpdateScene(matrizLaboratorio);
		rejasTecho->UpdateScene(matrizLaboratorio);

		// Banqueta
		banquetaPiso->UpdateScene(matrizLaboratorio);
		banquetaPiso2->UpdateScene(matrizLaboratorio);

		// Ventanas
		ventanas->UpdateScene(matrizLaboratorio);

		// Techo del invernadero
		invernaderoVentana->UpdateScene(matrizLaboratorio);
		invernaderoVidrio->UpdateScene(matrizLaboratorio);

		// Marcos
		marcosMarco->UpdateScene(matrizLaboratorio);
		marcosPB->UpdateScene(matrizLaboratorio);
		marcosPuerta->UpdateScene(matrizLaboratorio);
		marcosVentanas->UpdateScene(matrizLaboratorio);

		// Estructura del invernadero
		estructInvernadero->UpdateScene(matrizLaboratorio);

		// Piso de tierra del laboratorio
		pisoTierra->UpdateScene(matrizLaboratorio);

		// Techo principal
		techoPrincipal->UpdateScene(matrizLaboratorio);
		techoTierra->UpdateScene(matrizLaboratorio);

		// Orillas del techo
		techoOrillaMarco->UpdateScene(matrizLaboratorio);
		techoOrillaPB->UpdateScene(matrizLaboratorio);

		// Puertas
		puertasPuerta->UpdateScene(matrizLaboratorio);
		puertasMarco->UpdateScene(matrizLaboratorio);
		puertasPB->UpdateScene(matrizLaboratorio);

		// celdas criogenicas
		D3DXMATRIX posicionCelda1, posicionCelda2, posicionCelda3;
		float celdaX = 0.0f;
		float celdaY = 0.00f;
		float celdaZ = 0.0f;
		float separacion = 9.0f;

		// CELDA 1 
		D3DXMatrixTranslation(&posicionCelda1,
			celdaX, celdaY, celdaZ);
		matrizCelda1 = posicionCelda1 * matrizLaboratorio;
		// CELDA 2 
		D3DXMatrixTranslation(&posicionCelda2,
			celdaX, celdaY, celdaZ - separacion);
		matrizCelda2 = posicionCelda2 * matrizLaboratorio;
		// CELDA 3
		D3DXMatrixTranslation(&posicionCelda3,
			celdaX, celdaY, celdaZ - separacion * 2.0f);
		matrizCelda3 = posicionCelda3 * matrizLaboratorio;

		// PANELES DE CONTROL
		D3DXMATRIX posicionPanel1, posicionPanel2, posicionPanel3;
		D3DXMATRIX rotacionPanel, moverOrigen, regresarOrigen;

		// Posicion de los paneles
		float panelX = -20.0f;
		float panelY = 0.00f;
		float panelZ = -4.0f;


		// Separacion entre paneles
		float separacionPaneles = 9.0f;

		// CENTRO REAL DEL MODELO BTN.OBJ
		float centroX = 38.0706f;
		float centroZ = -114.3422f;

		// Llevar al centro de rotacion
		D3DXMatrixTranslation(&moverOrigen, -centroX, 0.0f, -centroZ);

		// Girar 180 grados sobre Y
		D3DXMatrixRotationY(&rotacionPanel, D3DXToRadian(180.0f));

		// Regresar a la posicion original
		D3DXMatrixTranslation(&regresarOrigen, centroX, 0.0f, centroZ);

		// PANEL 1
		D3DXMatrixTranslation(&posicionPanel1, panelX, panelY, panelZ);
		matrizPanel1 = moverOrigen * rotacionPanel * regresarOrigen * posicionPanel1 * matrizLaboratorio;

		// PANEL 2
		D3DXMatrixTranslation(&posicionPanel2, panelX, panelY, panelZ - separacionPaneles);
		matrizPanel2 = moverOrigen * rotacionPanel * regresarOrigen * posicionPanel2 * matrizLaboratorio;

		// PANEL 3
		D3DXMatrixTranslation(&posicionPanel3, panelX, panelY, panelZ - separacionPaneles * 2.0f);
		matrizPanel3 = moverOrigen * rotacionPanel * regresarOrigen * posicionPanel3 * matrizLaboratorio;
	

		// ESCRITORIO 

		D3DXMATRIX origenEscritorio;
		D3DXMATRIX escalaEscritorio;
		D3DXMATRIX rotacionEscritorio;
		D3DXMATRIX regresoEscritorio;
		D3DXMATRIX posicionEscritorio;

		float centroEscritorioX = 0.0f;
		float centroEscritorioY = 0.0f;
		float centroEscritorioZ = 0.0f;

		D3DXMatrixTranslation(&origenEscritorio,
			-centroEscritorioX,
			-centroEscritorioY,
			-centroEscritorioZ);

		D3DXMatrixScaling(&escalaEscritorio, 1.20f, 1.20f, 1.20f);

		D3DXMatrixRotationY(&rotacionEscritorio, D3DXToRadian(180.0f));

		D3DXMatrixTranslation(&regresoEscritorio,
			centroEscritorioX,
			centroEscritorioY,
			centroEscritorioZ);

		D3DXMatrixTranslation(&posicionEscritorio, 11.0f, 0.2f, 9.5f);

		matrizEscritorio = origenEscritorio * escalaEscritorio * rotacionEscritorio * regresoEscritorio * posicionEscritorio * matrizLaboratorio;

		escritorioDesk->UpdateScene(matrizEscritorio);
		escritorioDeskOld->UpdateScene(matrizEscritorio);
		escritorioKeyboardOld->UpdateScene(matrizEscritorio);
		escritorioKeybroard->UpdateScene(matrizEscritorio);
		escritorioGrid->UpdateScene(matrizEscritorio);
		escritorioGridOld->UpdateScene(matrizEscritorio);
		escritorioCables->UpdateScene(matrizEscritorio);
		escritorioCablesOld->UpdateScene(matrizEscritorio);
		escritorioScreen->UpdateScene(matrizEscritorio);
		escritorioScreenOld->UpdateScene(matrizEscritorio);
		escritorioSoloScreen->UpdateScene(matrizEscritorio);
		escritorioSoloScreenOld->UpdateScene(matrizEscritorio);

		// COMPUTADORAS

		D3DXMATRIX origenCompus, escalaCompus;
		D3DXMATRIX rotacionCompus, regresoCompus, posicionCompus;
		float centroCompusX = 3.953935f;
		float centroCompusY = 1.322478f;
		float centroCompusZ = -1.814324f;


		D3DXMatrixTranslation(&origenCompus,
			-centroCompusX, -centroCompusY, -centroCompusZ);

		// Escala 
		D3DXMatrixScaling(&escalaCompus, 1.5f, 1.5f, 1.5f);

		// Rotacion 
		D3DXMatrixRotationY(&rotacionCompus, D3DXToRadian(180.0f));

		D3DXMatrixTranslation(&regresoCompus,
			centroCompusX, centroCompusY, centroCompusZ);

		// Posicion 
		D3DXMatrixTranslation(&posicionCompus, .0f, 0.0f, 0.0f);

		// Matriz final
		matrizCompus = origenCompus * escalaCompus * rotacionCompus *
			regresoCompus * posicionCompus * matrizLaboratorio;

		compus->UpdateScene(matrizCompus);




		BoundingSphere1->UpdateScene(Bounding1 * MatAcomodo * BoundinMatTranslation);
		BoundingSphere2->UpdateScene(Bounding2 * MatAcomodo);


		if (CheckBoundingSphereCollision(BoundingSphere1->PrimitiveBoundings->Radius,
			BoundingSphere1->PrimitiveBoundings->PositionM, BoundingSphere1->getWorldMatrix(),
			BoundingSphere2->PrimitiveBoundings->Radius,
			BoundingSphere2->PrimitiveBoundings->PositionM, BoundingSphere2->getWorldMatrix())) {

			BoundingSphereTranslation = LastBoundingPosition;
		}


		DummyBoundyEvents.x = 0;
		DummyBoundyEvents.y = 0;

		// RECOGER ITEMS
		itemCercano = -1;
		float distMin = 8.0f;
		for (int i = 0; i < numItems; i++)
		{
			if (items[i].recogido) continue;

			D3DXMATRIX esc, tras, mat;
			D3DXMatrixScaling(&esc, items[i].escala, items[i].escala, items[i].escala);
			D3DXMatrixTranslation(&tras, items[i].pos.x, items[i].pos.y, items[i].pos.z);
			mat = esc * tras;
			items[i].modelo->UpdateScene(mat);

			D3DXVECTOR3 dif = camara->Position - items[i].pos;
			dif.y = 0;
			float dist = D3DXVec3Length(&dif);
			if (dist < distMin) { distMin = dist; itemCercano = i; }
		}

		if (itemCercano >= 0)
		{
			D3DXVECTOR3 p = items[itemCercano].pos;
			D3DXMATRIX matIcono;
			D3DXMatrixTranslation(&matIcono, p.x, p.y + 2.5f + sin(rotPerFrame * 0.05f) * 0.3f, p.z);
			iconoTomar->UpdateScene(matIcono);
		}

	}

	void DrawScene()
	{
		//Clear our backbuffer to the updated color
		D3DXCOLOR bgColor(0.0f, 0.0f, 0.25f, 1.0f);
		d3d11DevCon->ClearRenderTargetView(renderTargetView, bgColor);
		//Refresh the Depth/Stencil view
		d3d11DevCon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		cubo->DrawScene(camara);
		esfera->DrawScene(camara);
		toroide->DrawScene(camara);
		cilindro->DrawScene(camara);

		cono->DrawScene(camara);
		billboard->DrawScene(camara);

		//Lab estructura
		// PAREDES SUPERIORES Y LETREROS
		paredesArriba->DrawScene(camara);
		marcosArriba->DrawScene(camara);
		paredesArribaVentanas->DrawScene(camara);

		letreroLet1->DrawScene(camara);
		letreroLet3->DrawScene(camara);
		letreroLet4->DrawScene(camara);
		letreroLet5->DrawScene(camara);
		letreroLet6->DrawScene(camara);
		letreroLet7->DrawScene(camara);
		letreroLet8->DrawScene(camara);
		letreroLet9->DrawScene(camara);
		letreroLet10->DrawScene(camara);
		letreroLet11->DrawScene(camara);

		franja->DrawScene(camara);
		paredesAbajo->DrawScene(camara);
		paredesAbajoPB->DrawScene(camara);
		piso->DrawScene(camara);
		rejasMetal->DrawScene(camara);
		rejasFranja->DrawScene(camara);
		rejasTierra->DrawScene(camara);
		rejasTecho->DrawScene(camara);
		banquetaPiso->DrawScene(camara);
		banquetaPiso2->DrawScene(camara);
		ventanas->DrawScene(camara);
		invernaderoVentana->DrawScene(camara);
		invernaderoVidrio->DrawScene(camara);
		marcosMarco->DrawScene(camara);
		marcosPB->DrawScene(camara);
		marcosPuerta->DrawScene(camara);
		estructInvernadero->DrawScene(camara);
		pisoTierra->DrawScene(camara);
		techoPrincipal->DrawScene(camara);
		techoTierra->DrawScene(camara);
		techoOrillaMarco->DrawScene(camara);
		techoOrillaPB->DrawScene(camara);
		puertasPuerta->DrawScene(camara);
		puertasMarco->DrawScene(camara);
		puertasPB->DrawScene(camara);
		marcosVentanas->DrawScene(camara);
		// CELDA 1
		celdaMetal->UpdateScene(matrizCelda1);
		celdaVidrio->UpdateScene(matrizCelda1);
		celdaLuces->UpdateScene(matrizCelda1);

		celdaMetal->DrawScene(camara);
		celdaVidrio->DrawScene(camara);
		celdaLuces->DrawScene(camara);

		// CELDA 2
		celdaMetal->UpdateScene(matrizCelda2);
		celdaVidrio->UpdateScene(matrizCelda2);
		celdaLuces->UpdateScene(matrizCelda2);

		celdaMetal->DrawScene(camara);
		celdaVidrio->DrawScene(camara);
		celdaLuces->DrawScene(camara);

		// CELDA 3
		celdaMetal->UpdateScene(matrizCelda3);
		celdaVidrio->UpdateScene(matrizCelda3);
		celdaLuces->UpdateScene(matrizCelda3);

		celdaMetal->DrawScene(camara);
		celdaVidrio->DrawScene(camara);
		celdaLuces->DrawScene(camara);

		panelMetal->UpdateScene(matrizPanel1);
		panelPared->UpdateScene(matrizPanel1);
		panelPantalla->UpdateScene(matrizPanel1);
		panelBotonRojo->UpdateScene(matrizPanel1);

		panelMetal->DrawScene(camara);
		panelPared->DrawScene(camara);
		panelPantalla->DrawScene(camara);
		panelBotonRojo->DrawScene(camara);

		panelMetal->UpdateScene(matrizPanel2);
		panelPared->UpdateScene(matrizPanel2);
		panelPantalla->UpdateScene(matrizPanel2);
		panelBotonVerde->UpdateScene(matrizPanel2);

		panelMetal->DrawScene(camara);
		panelPared->DrawScene(camara);
		panelPantalla->DrawScene(camara);
		panelBotonVerde->DrawScene(camara);

		panelMetal->UpdateScene(matrizPanel3);
		panelPared->UpdateScene(matrizPanel3);
		panelPantalla->UpdateScene(matrizPanel3);
		panelBotonAzul->UpdateScene(matrizPanel3);

		panelMetal->DrawScene(camara);
		panelPared->DrawScene(camara);
		panelPantalla->DrawScene(camara);
		panelBotonAzul->DrawScene(camara);

		// DIBUJAR ESCRITORIO
		escritorioDesk->DrawScene(camara);
		escritorioDeskOld->DrawScene(camara);
		escritorioKeyboardOld->DrawScene(camara);
		escritorioKeybroard->DrawScene(camara);
		escritorioGrid->DrawScene(camara);
		escritorioGridOld->DrawScene(camara);
		escritorioCables->DrawScene(camara);
		escritorioCablesOld->DrawScene(camara);
		escritorioScreen->DrawScene(camara);
		escritorioScreenOld->DrawScene(camara);
		escritorioSoloScreen->DrawScene(camara);
		escritorioSoloScreenOld->DrawScene(camara);
		compus->DrawScene(camara);




		terreno->DrawScene(camara);
		skydomo->DrawScene(skyCamara);

		// RECOGER ITEMS dibujarlos
		for (int i = 0; i < numItems; i++)
			if (!items[i].recogido) items[i].modelo->DrawScene(camara);

		BoundingSphere1->DrawScene(camara);
		BoundingSphere2->DrawScene(camara);

		if (itemCercano >= 0) iconoTomar->DrawScene(camara);

		if (itemEnMano >= 0)
		{
			D3DXMATRIX esc, offset, rot, tras, mat;
			float e = items[itemEnMano].escala * 0.5f;
			D3DXMatrixScaling(&esc, e, e, e);
			D3DXMatrixTranslation(&offset, 1.2f, -1.0f, 2.5f);
			D3DXMatrixRotationYawPitchRoll(&rot, camara->Yaw, camara->Pitch, 0);
			D3DXMatrixTranslation(&tras, camara->Position.x, camara->Position.y, camara->Position.z);
			mat = esc * offset * rot * tras;
			items[itemEnMano].modelo->UpdateScene(mat);
			items[itemEnMano].modelo->DrawScene(camara);
		}

		// fuego 
		for (int i = 0; i < numFuegos; i++)
		{
			if (!fuegos[i].activo) continue;
			float t = fuegos[i].tam * (1.0f + sin(rotPerFrame * 0.3f + i * 1.7f) * 0.05f);
			D3DXMATRIX escF, trasF, matF;
			D3DXMatrixScaling(&escF, t, t, 1.0f);
			D3DXMatrixTranslation(&trasF, fuegos[i].pos.x, fuegos[i].pos.y, fuegos[i].pos.z);
			matF = escF * trasF;
			int cuadro = (GetTickCount() / 120 + i) % 3;
			fuego[cuadro]->UpdateScene(matF);
			fuego[cuadro]->DrawScene(camara);
		}

		// gui, camra posx popsy ancho alto
		hudVida->DrawBitmap(camara, -0.90f, 0.85f, 0.30f, 0.30f);  // arriba a la izquierda
		hudMapa->DrawBitmap(camara, 0.80f, -0.70f, 0.30f, 0.30f);  // abajo a la derecha

		// RECOGER ITEMS pero el hud
		float x = -0.85f;
		for (int i = 0; i < numItems; i++)
		{
			if (!items[i].recogido) continue;
			float alto = (i == itemEnMano) ? 0.30f : 0.22f;
			items[i].icono->DrawBitmap(camara, x, -0.82f, alto * 0.75f, alto);
			x += 0.22f;
		}

		//Present the backbuffer to the screen
		SwapChain->Present(0, 0);
	}


	bool CheckBoundingSphereCollision(float firstSphereRadius,
		D3DXVECTOR3 firstSpherePos,
		D3DXMATRIX firstObjWorldSpace,
		float secondSphereRadius,
		D3DXVECTOR3 secondSpherePos,
		D3DXMATRIX secondObjWorldSpace)
	{
		//Declare local variables
		D3DXVECTOR3 world_1 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 world_2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		float objectsDistance = 0.0f;

		//Transform the objects world space to objects REAL center in world space
		D3DXVec3TransformCoord(&world_1, &firstSpherePos, &firstObjWorldSpace);
		D3DXVec3TransformCoord(&world_2, &secondSpherePos, &secondObjWorldSpace);

		// Create Vector to verify distance
		world_1 -= world_2;

		//Get the distance between the two objects
		objectsDistance = D3DXVec3Length(&world_1);
		//objectsDistance = XMVectorGetX(XMVector3Length(world_1 - world_2));

		//If the distance between the two objects is less than the sum of their bounding spheres...
		if (objectsDistance <= (firstSphereRadius + secondSphereRadius))
			return true;

		//If the bounding spheres are not colliding, return false
		return false;
	}


};

#endif