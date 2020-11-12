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

#define ANIMACAO_FASE_CONTRACT	0
#define ANIMACAO_FASE_DOWN		1
#define ANIMACAO_FASE_PASSO		2
#define ANIMACAO_FASE_UP		3

#define ANIMACAO_MODO_PARADO 0
#define ANIMACAO_MODO_CAORRENDO 1
#define ANIMACAO_MODO_ANDANDO 2


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
	/*
	 * o angulo baseado da cabeça aos outros membros
	*/
	float cabeca;
	float ombro1;
	float ombro2;
	float ombroQ;
	float cotovelo1;
	float cotovelo2;
	float quadril1;
	float quadril2;
	float joelho1;
	float joelho2;
	float tornozelo1;
	float tornozelo2;
}STICKMAN_ANGULOS;

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
	STICKMAN_ANGULOS ang;
}STICKMAN;



















#endif // deve ser a ultima linha do arquivo
