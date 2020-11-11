#ifndef PARAMETROS_H
#define PARAMETROS_H

#define PI 3.1415

#define SET_TIMER_60 60

#define T_COLOR_RED (COR){1.0f, 0.0f, 0.0f}
#define T_COLOR_WHITE (COR){1.0f, 1.0f, 1.0f}

#define RAIO_CABECA 0.11f
#define RAIO_JUNTA 0.013f
#define H_PESCOCO		0.20f
#define H_BRACOS		0.16f
#define H_ANTEBRACO		0.16f
#define H_TRONCO		0.30f
#define H_COXAS			0.16f
#define H_PANTURRILHA	0.16f
#define H_PES			0.03f

#define distanciax(r,a,p) ((cos(2*PI*a/360)*(r))+p)
#define distanciay(r,a,p) ((sin(2*PI*a/360)*(r))+p)

typedef struct {
	float R;
	float G;
	float B;
} COR;

typedef struct {
	float x;
	float y;
}PONTO;

typedef struct{
	PONTO cabeca;
	PONTO ombro;
	PONTO quadril;
	PONTO cotovelo1;
	PONTO cotovelo2;
	PONTO mao1;
	PONTO mao2;
	PONTO joelho1;
	PONTO joelho2;
	PONTO calcanhar1;
	PONTO calcanhar2;
	PONTO pe1;
	PONTO pe2;
}STICKMAN;



















#endif // deve ser a ultima linha do arquivo
