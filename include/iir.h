/* ==================================================================
 File name       : IIR.H                     

 Originator      : Advanced Embeeded Control (AEC)
 Texas Instruments Inc.

 Description:  
 Header file containing object definitions, proto type declaration and 
 default object initializers for IIR Filter.
 ===================================================================
 History:
 -------------------------------------------------------------------
 12-05-2003    Release Rev 1.0                                                  
 -------------------------------------------------------------------*/

#ifndef __IIR_H__
#define __IIR_H__
#define     NULL    0

/*----------------------------------------------------------------
 Define the structure of the IIR5BIQ16 Filter Module 
 -----------------------------------------------------------------*/
typedef struct {
	void (*init)(void *); /* Ptr to Init funtion           */
	void (*calc)(void *); /* Ptr to calc fn                */
	int *coeff_ptr; /* Pointer to Filter coefficient */
	int *dbuffer_ptr; /* Delay buffer ptr              */
	int nbiq; /* No of biquad                  */
	int input; /* Latest Input sample           */
	int isf; /* Input Scale Factor            */
	int qfmat; /* Q format of coeff             */
	int output; /* Filter Output                 */
} IIR5BIQ16;

/*---------------------------------------------------------------
 Define the structure of the IIR5BIQ32 Filter Module
 -----------------------------------------------------------------*/
typedef struct {
	void (*init)(void *); /* Ptr to init fn                */
	void (*calc)(void *); /* Ptr to calc fn                */
	long *coeff_ptr; /* Pointer to Filter coefficient */
	long *dbuffer_ptr; /* Delay buffer ptr              */
	int nbiq; /* No of biquad                  */
	int input; /* Latest Input sample           */
	long isf; /* Input Scale Factor            */
	long output32; /* Filter Output (Q30)           */
	int output16; /* Filter Output (Q14)           */
	int qfmat; /* Q format of coeff             */
} IIR5BIQ32;

/*---------------------------------------------------------------
 Define a Handles for the Filter Modules
 -----------------------------------------------------------------*/
typedef IIR5BIQ16 *IIR5BIQ16_handle;
typedef IIR5BIQ32 *IIR5BIQ32_handle;

#define IIR5BIQ16_DEFAULTS { (void (*)(void *))IIR5BIQ16_init,\
             (void (*)(void *))IIR5BIQ16_calc,\
             (int *)NULL,   \
             (int *)NULL,   \
             0,             \
             0,             \
             0,             \
             0,             \
             0}    

#define IIR5BIQ32_DEFAULTS { (void (*)(void *))IIR5BIQ32_init,\
             (void (*)(void *))IIR5BIQ32_calc, \
             (long *)NULL,   \
             (long *)NULL,   \
             0,             \
             0,             \
             0,             \
             0,             \
             0,             \
             0}                           

/*-------------------------------------------------------------
 Prototypes for the functions
 ---------------------------------------------------------------*/
void IIR5BIQ16_calc(void *);
void IIR5BIQ32_calc(void *);

/******** Sample IIR co-efficients ****************************/

void IIR5BIQ16_init(IIR5BIQ16 *);
void IIR5BIQ32_init(IIR5BIQ32 *);

/*  ȫ�ֹ��ܺ������� */
extern void IIR32_Init(IIR5BIQ32_handle iir, long dbuffer[], long coeff[],
		int qfmat, int nbiq, long isf);
extern int IIR32_Calc(IIR5BIQ32_handle iir, int xn);

/*  BSF300����300Hz�ݲ�������  (�������̫����)*/
#define BSF300_COEFF {\
			-2045604,4069698,1197127,-2352031,1197127,\
			-2082247,4098235,2447300,-4805392,2447300,\
			-2083601,4113806,855003360,-1680828169,855003360}

#define BSF300_ISF	7575
#define BSF300_NBIQ	3
#define BSF300_QFMAT	21

/*  BSF300ȫ�ֱ�������  */
extern long BSF300_coeff[5 * BSF300_NBIQ];
extern int BSF300_qfmat;
extern int BSF300_nbiq;
extern long BSF300_isf;

/*  BSF600����600Hz�ݲ�������  */
#define BSF600_COEFF {\
			-4091209,7704283,2290751,-4260108,2290751,\
			-4165196,7744444,5228934,-9712193,5228934,\
			-4166500,7801647,849065447,-1580933792,849065447}

#define BSF600_ISF	29851
#define BSF600_NBIQ	3
#define BSF600_QFMAT	22

/*  BSF600ȫ�ֱ�������  */
extern long BSF600_coeff[5 * BSF600_NBIQ];
extern int BSF600_qfmat;
extern int BSF600_nbiq;
extern long BSF600_isf;

/*  LPF150 ����  ʵ��LPFת��Ƶ��150Hz*/
#define LPF150_COEFF {\
			0,488493505,0,547750,547750,\
			-449594600,982083486,1110263,2220526,1110263,\
			-464637639,997059702,1088221,2176442,1088221,\
			-488596124,1020911760,720293,1440587,720293,\
			-519598389,1051776308,916753101,1833506201,916753101}

#define LPF150_ISF	48377407
#define LPF150_NBIQ	5
#define LPF150_QFMAT	29

/*  LPF150ȫ�ֱ�������  */
extern long LPF150_coeff[5 * LPF150_NBIQ];
extern int LPF150_qfmat;
extern int LPF150_nbiq;
extern long LPF150_isf;

#endif 

