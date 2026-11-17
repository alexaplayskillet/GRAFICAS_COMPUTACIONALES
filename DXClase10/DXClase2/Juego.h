#ifndef _Juego_
#define _Juego_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "MapaColision.h"

#define NUM_PUERTAS 9
#define NUM_CAJAS 8
#define COLS_FUEGO 11
#define FILAS_FUEGO 9
#define NUM_CASILLAS 99
#define NUM_HORAS 6
#define HORA_SEG 90.0f
#define TIEMPO_BOTON 10.0f
#define TIEMPO_ESCONDITE 60.0f
#define PROPAGA_SEG 3.0f
#define PAUSA_SEG 8.0f
#define RADIO_JUGADOR 1.4f
#define DENSIDAD_NIEBLA 0.018f

class Juego
{
public:
	struct Puerta
	{
		int eje;
		float c, a, b;
		float abierta;
		bool abrir;
	};
	Puerta puertas[NUM_PUERTAS];

	D3DXVECTOR3 carpetas[3];
	bool carpetaLeida[3];
	bool hudCarpeta;
	int carpetaMostrada;

	int estado;
	int nivel;
	int causa;
	float tiempo;
	float tPerder;
	bool reiniciado;
	bool promptE;

	//capsulas
	int ordenCapsulas[3];
	int pulsados[3];
	int numPulsados;
	float tPulsado[3];
	float tFalloCapsula;
	bool capsulaEncendida;
	D3DXVECTOR3 botonPos[3];

	//escondite
	D3DXVECTOR3 cajas[NUM_CAJAS];
	int cajaMonstruo;
	int monstruo;
	float tEscondite;
	float tJumpscare;

	//mago
	bool fuego[NUM_CASILLAS];
	bool excluida[NUM_CASILLAS];
	float casillaX[NUM_CASILLAS];
	float casillaZ[NUM_CASILLAS];
	int encendidas;
	int totalCasillas;
	float tPropaga;
	float tPausa;

	//auto
	bool enAuto;
	float autoX, autoZ, autoYaw, autoVel;
	float autoPrevX, autoPrevZ;

	unsigned char* mapa;

	Juego()
	{
		srand((unsigned int)time(NULL));

		mapa = new unsigned char[MAPA_NX * MAPA_NZ];
		int pos = 0;
		unsigned char v = 0;
		for (int i = 0; i < MAPA_NRUNS; i++)
		{
			for (int k = 0; k < MAPA_RUNS[i] && pos < MAPA_NX * MAPA_NZ; k++)
				mapa[pos++] = v;
			v = 1 - v;
		}

		float datos[NUM_PUERTAS][4] = {
			{ 1, -0.8f, -10.1f, -0.9f },
			{ 1, 70.8f, -176.9f, -167.7f },
			{ 0, 24.1f, 106.4f, 117.9f },
			{ 0, -22.5f, 106.4f, 117.9f },
			{ 0, -132.2f, 106.4f, 117.9f },
			{ 0, -149.7f, 22.8f, 31.8f },
			{ 0, -99.4f, 22.8f, 31.7f },
			{ 0, -31.8f, 22.8f, 32.0f },
			{ 0, -85.0f, 103.4f, 120.1f }
		};
		for (int i = 0; i < NUM_PUERTAS; i++)
		{
			puertas[i].eje = (int)datos[i][0];
			puertas[i].c = datos[i][1];
			puertas[i].a = datos[i][2];
			puertas[i].b = datos[i][3];
		}

		carpetas[0] = D3DXVECTOR3(33.3f, 3.4f, -99.05f);
		carpetas[1] = D3DXVECTOR3(70.3f, 3.4f, -166.9f);
		carpetas[2] = D3DXVECTOR3(119.9f, 3.4f, -93.0f);

		float cx[NUM_CAJAS] = { 88, 112, 138, 98, 124, 88, 112, 140 };
		float cz[NUM_CAJAS] = { -160, -154, -160, -178, -182, -192, -191, -190 };
		for (int i = 0; i < NUM_CAJAS; i++)
			cajas[i] = D3DXVECTOR3(cx[i], 2.2f, cz[i]);

		for (int i = 0; i < 3; i++)
			botonPos[i] = D3DXVECTOR3(18.9f, 4.5f, -116.0f - 9.0f * i);

		totalCasillas = 0;
		for (int j = 0; j < FILAS_FUEGO; j++)
		{
			for (int i = 0; i < COLS_FUEGO; i++)
			{
				int k = j * COLS_FUEGO + i;
				casillaX[k] = 74.2f + (i + 0.5f) * 6.8f;
				casillaZ[k] = -84.0f + (j + 0.5f) * 6.8f;
				float dx = casillaX[k] - 111.7f;
				float dz = casillaZ[k] + 54.0f;
				excluida[k] = sqrtf(dx * dx + dz * dz) < 12.5f;
				if (!excluida[k]) totalCasillas++;
			}
		}

		autoX = 0; autoZ = 0; autoYaw = 0; autoVel = 0;
		Reiniciar(true);
	}

	void Reiniciar(bool inicio)
	{
		estado = EstadoPasillo;
		nivel = 0;
		causa = 0;
		tiempo = 0.0f;
		tPerder = 0.0f;
		promptE = false;
		hudCarpeta = false;
		carpetaMostrada = 0;
		for (int i = 0; i < NUM_PUERTAS; i++)
		{
			puertas[i].abierta = 0.0f;
			puertas[i].abrir = false;
		}
		for (int i = 0; i < 3; i++) carpetaLeida[i] = false;
		OrdenarCapsulas();
		cajaMonstruo = rand() % NUM_CAJAS;
		monstruo = 0;
		tEscondite = TIEMPO_ESCONDITE;
		tJumpscare = 0.0f;
		LimpiarFuego();
		enAuto = false;
		autoVel = 0.0f;
		puertas[7].abrir = true;
		reiniciado = !inicio;
	}

	void OrdenarCapsulas()
	{
		for (int i = 0; i < 3; i++) ordenCapsulas[i] = i;
		for (int i = 2; i > 0; i--)
		{
			int j = rand() % (i + 1);
			int t = ordenCapsulas[i];
			ordenCapsulas[i] = ordenCapsulas[j];
			ordenCapsulas[j] = t;
		}
		numPulsados = 0;
		capsulaEncendida = false;
		tFalloCapsula = 0.0f;
		for (int i = 0; i < 3; i++) { pulsados[i] = -1; tPulsado[i] = 0.0f; }
	}

	void LimpiarFuego()
	{
		for (int i = 0; i < NUM_CASILLAS; i++) fuego[i] = false;
		encendidas = 0;
		tPropaga = 0.0f;
		tPausa = 0.0f;
	}

	bool EnZona(D3DXVECTOR3 p, int n)
	{
		float z[3][4] = {
			{ 12.0f, 44.0f, -148.0f, -102.0f },
			{ 73.0f, 153.0f, -210.0f, -134.0f },
			{ 75.0f, 148.0f, -83.0f, -24.0f }
		};
		return p.x > z[n][0] && p.x < z[n][1] && p.z > z[n][2] && p.z < z[n][3];
	}

	float Dist2D(D3DXVECTOR3 a, D3DXVECTOR3 b)
	{
		float dx = a.x - b.x;
		float dz = a.z - b.z;
		return sqrtf(dx * dx + dz * dz);
	}

	int Hora()
	{
		int h = (int)(tiempo / HORA_SEG);
		return h > NUM_HORAS - 1 ? NUM_HORAS - 1 : h;
	}

	void Perder(int c)
	{
		if (estado == EstadoPerder || estado == EstadoGanar) return;
		estado = EstadoPerder;
		causa = c;
		tPerder = 0.0f;
		hudCarpeta = false;
	}

	void Ganar()
	{
		if (estado == EstadoGanar) return;
		estado = EstadoGanar;
		tPerder = 0.0f;
	}

	void CompletarNivel()
	{
		if (estado == EstadoCapsula) puertas[5].abrir = true;
		if (estado == EstadoEscondite) puertas[4].abrir = true;
		if (estado == EstadoMago)
		{
			puertas[3].abrir = true;
			puertas[2].abrir = true;
		}
		nivel++;
		estado = EstadoPasillo;
	}

	void IniciarNivel(int n)
	{
		if (n == 0)
		{
			estado = EstadoCapsula;
			OrdenarCapsulas();
		}
		if (n == 1)
		{
			estado = EstadoEscondite;
			cajaMonstruo = rand() % NUM_CAJAS;
			monstruo = 0;
			tEscondite = TIEMPO_ESCONDITE;
			tJumpscare = 0.0f;
		}
		if (n == 2)
		{
			estado = EstadoMago;
			IniciarMago();
		}
	}

	void IniciarMago()
	{
		LimpiarFuego();
		int anillo[NUM_CASILLAS];
		int n = 0;
		for (int j = 0; j < FILAS_FUEGO; j++)
		{
			for (int i = 0; i < COLS_FUEGO; i++)
			{
				int k = j * COLS_FUEGO + i;
				if (excluida[k]) continue;
				bool junto = false;
				for (int dj = -1; dj <= 1; dj++)
					for (int di = -1; di <= 1; di++)
					{
						int ni = i + di, nj = j + dj;
						if (ni < 0 || nj < 0 || ni >= COLS_FUEGO || nj >= FILAS_FUEGO) continue;
						if (excluida[nj * COLS_FUEGO + ni]) junto = true;
					}
				if (junto) anillo[n++] = k;
			}
		}
		int a = rand() % n;
		int b = (a + 1 + rand() % (n - 1)) % n;
		fuego[anillo[a]] = true;
		fuego[anillo[b]] = true;
		encendidas = 2;
	}

	void PropagarFuego()
	{
		bool nuevo[NUM_CASILLAS];
		for (int k = 0; k < NUM_CASILLAS; k++) nuevo[k] = false;
		for (int j = 0; j < FILAS_FUEGO; j++)
		{
			for (int i = 0; i < COLS_FUEGO; i++)
			{
				if (!fuego[j * COLS_FUEGO + i]) continue;
				int libres[8];
				int n = 0;
				for (int dj = -1; dj <= 1; dj++)
					for (int di = -1; di <= 1; di++)
					{
						if (di == 0 && dj == 0) continue;
						int ni = i + di, nj = j + dj;
						if (ni < 0 || nj < 0 || ni >= COLS_FUEGO || nj >= FILAS_FUEGO) continue;
						int k = nj * COLS_FUEGO + ni;
						if (!excluida[k] && !fuego[k]) libres[n++] = k;
					}
				if (n > 0) nuevo[libres[rand() % n]] = true;
			}
		}
		for (int k = 0; k < NUM_CASILLAS; k++)
			if (nuevo[k]) fuego[k] = true;
	}

	void ApagarFuego(int k)
	{
		if (k < 0 || k >= NUM_CASILLAS || !fuego[k]) return;
		fuego[k] = false;
		tPausa = PAUSA_SEG;
	}

	void PresionarBoton(int k)
	{
		if (capsulaEncendida || numPulsados >= 3) return;
		if (k == ordenCapsulas[numPulsados])
		{
			pulsados[numPulsados] = k;
			tPulsado[numPulsados] = 0.0f;
			numPulsados++;
			if (numPulsados == 3)
			{
				capsulaEncendida = true;
				CompletarNivel();
			}
		}
		else
		{
			numPulsados = 0;
			tFalloCapsula = 0.7f;
		}
	}

	void RevisarCaja(int k)
	{
		if (monstruo != 0) return;
		if (k == cajaMonstruo)
		{
			monstruo = 2;
			CompletarNivel();
		}
	}

	int Buscar(D3DXVECTOR3 pm, float distAuto, int* idx)
	{
		*idx = -1;
		if (enAuto) return 0;
		if (estado == EstadoPasillo)
		{
			if (nivel < 3 && Dist2D(pm, carpetas[nivel]) < 6.5f) { *idx = nivel; return 1; }
			if (nivel == 3 && distAuto < 11.0f) return 4;
		}
		if (estado == EstadoCapsula && !capsulaEncendida)
		{
			float dMin = 5.5f;
			for (int k = 0; k < 3; k++)
			{
				float d = Dist2D(pm, botonPos[k]);
				if (d < dMin) { dMin = d; *idx = k; }
			}
			if (*idx >= 0) return 2;
		}
		if (estado == EstadoEscondite && monstruo == 0)
		{
			float dMin = 6.0f;
			for (int k = 0; k < NUM_CAJAS; k++)
			{
				float d = Dist2D(pm, cajas[k]);
				if (d < dMin) { dMin = d; *idx = k; }
			}
			if (*idx >= 0) return 3;
		}
		return 0;
	}

	bool PulsarE(D3DXVECTOR3 pm, float distAuto)
	{
		if (estado == EstadoPerder)
		{
			if (tPerder > 1.0f) Reiniciar(false);
			return true;
		}
		if (estado == EstadoGanar)
		{
			if (tPerder > 3.0f) Reiniciar(false);
			return true;
		}
		if (monstruo == 1 && estado == EstadoEscondite) return true;
		if (hudCarpeta)
		{
			int entrada[3] = { 6, 1, 8 };
			hudCarpeta = false;
			carpetaLeida[carpetaMostrada] = true;
			puertas[entrada[carpetaMostrada]].abrir = true;
			return true;
		}
		int idx;
		int que = Buscar(pm, distAuto, &idx);
		if (que == 1) { hudCarpeta = true; carpetaMostrada = idx; return true; }
		if (que == 2) { PresionarBoton(idx); return true; }
		if (que == 3) { RevisarCaja(idx); return true; }
		if (que == 4) { enAuto = true; autoVel = 0.0f; return true; }
		return false;
	}

	void Actualizar(float dt, D3DXVECTOR3 pm, float distAuto)
	{
		for (int i = 0; i < NUM_PUERTAS; i++)
		{
			if (puertas[i].abrir && puertas[i].abierta < 1.0f)
			{
				puertas[i].abierta += dt / 1.6f;
				if (puertas[i].abierta > 1.0f) puertas[i].abierta = 1.0f;
			}
		}

		if (estado == EstadoGanar || estado == EstadoPerder)
		{
			tPerder += dt;
			promptE = tPerder > (estado == EstadoGanar ? 3.0f : 1.0f);
			if (estado == EstadoPerder && tPerder > 15.0f) Reiniciar(false);
			return;
		}

		tiempo += dt;
		if (tiempo >= HORA_SEG * NUM_HORAS)
		{
			Perder(0);
			return;
		}

		int idx;
		promptE = hudCarpeta || Buscar(pm, distAuto, &idx) != 0;

		if (estado == EstadoPasillo)
		{
			if (nivel < 3 && carpetaLeida[nivel] && EnZona(pm, nivel)) IniciarNivel(nivel);
		}
		else if (estado == EstadoCapsula)
		{
			if (tFalloCapsula > 0.0f) tFalloCapsula -= dt;
			if (numPulsados > 0)
			{
				for (int i = 0; i < numPulsados; i++) tPulsado[i] += dt;
				if (tPulsado[0] > TIEMPO_BOTON) numPulsados = 0;
			}
		}
		else if (estado == EstadoEscondite)
		{
			if (monstruo == 0)
			{
				tEscondite -= dt;
				if (tEscondite <= 0.0f)
				{
					tEscondite = 0.0f;
					monstruo = 1;
					tJumpscare = 0.0f;
				}
			}
			else if (monstruo == 1)
			{
				tJumpscare += dt;
				if (tJumpscare >= 2.2f) Perder(1);
			}
		}
		else if (estado == EstadoMago)
		{
			if (tPausa > 0.0f) tPausa -= dt;
			else
			{
				tPropaga += dt;
				if (tPropaga >= PROPAGA_SEG)
				{
					tPropaga = 0.0f;
					PropagarFuego();
				}
			}
			encendidas = 0;
			for (int k = 0; k < NUM_CASILLAS; k++)
				if (fuego[k]) encendidas++;
			if (encendidas == 0) CompletarNivel();
			else if (encendidas >= totalCasillas) Perder(2);
		}
	}

	bool Prohibido(float x, float z)
	{
		int i = (int)floorf((x - MAPA_X0) / MAPA_RES);
		int j = (int)floorf((z - MAPA_Z0) / MAPA_RES);
		if (i >= 0 && j >= 0 && i < MAPA_NX && j < MAPA_NZ && mapa[j * MAPA_NX + i]) return true;

		float t = 0.6f + RADIO_JUGADOR;
		for (int k = 0; k < NUM_PUERTAS; k++)
		{
			Puerta& p = puertas[k];
			if (p.abierta > 0.8f) continue;
			if (p.eje == 0 && fabsf(z - p.c) < t && x > p.a - 0.3f && x < p.b + 0.3f) return true;
			if (p.eje == 1 && fabsf(x - p.c) < t && z > p.a - 0.3f && z < p.b + 0.3f) return true;
		}
		for (int k = 0; k < NUM_CAJAS; k++)
		{
			if (fabsf(x - cajas[k].x) < 2.0f + RADIO_JUGADOR && fabsf(z - cajas[k].z) < 2.0f + RADIO_JUGADOR)
				return true;
		}
		return false;
	}

	bool Resolver(D3DXVECTOR3 antes, D3DXVECTOR3& nuevo)
	{
		if (!Prohibido(nuevo.x, nuevo.z)) return false;
		if (Prohibido(antes.x, antes.z)) return false;
		if (!Prohibido(nuevo.x, antes.z)) { nuevo.z = antes.z; return true; }
		if (!Prohibido(antes.x, nuevo.z)) { nuevo.x = antes.x; return true; }
		nuevo.x = antes.x;
		nuevo.z = antes.z;
		return true;
	}

	bool EnEdificio(D3DXVECTOR3 pm)
	{
		return pm.x > -3.0f && pm.x < 159.0f && pm.z > -215.0f && pm.z < 26.5f;
	}

	void AutoColocar(float x, float z, float yaw)
	{
		autoX = x; autoZ = z; autoYaw = yaw; autoVel = 0.0f;
	}

	void AutoMover(float dt, float acel, float giro)
	{
		autoPrevX = autoX;
		autoPrevZ = autoZ;
		float maxV = 32.0f;
		autoVel += acel * 26.0f * dt;
		if (acel == 0.0f)
		{
			float f = 14.0f * dt;
			if (autoVel > 0.0f) autoVel = autoVel > f ? autoVel - f : 0.0f;
			else autoVel = autoVel < -f ? autoVel + f : 0.0f;
		}
		if (autoVel > maxV) autoVel = maxV;
		if (autoVel < -12.0f) autoVel = -12.0f;
		autoYaw += giro * 1.8f * dt * (autoVel / maxV);
		autoX += sinf(autoYaw) * autoVel * dt;
		autoZ += cosf(autoYaw) * autoVel * dt;
		if (autoX < 12.0f || autoX > 370.0f || autoZ < 12.0f || autoZ > 370.0f) Ganar();
	}

	void AutoRevertir()
	{
		autoX = autoPrevX;
		autoZ = autoPrevZ;
		autoVel = 0.0f;
	}
};

#endif
