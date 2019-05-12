
#include "alignment_parameters.h"

int set_subm_gaps(struct parameters* param, struct aln_param* ap);

struct aln_param* init_ap(struct parameters* param,int numseq)
{
        struct aln_param* ap = NULL;
        int i,j;


        MMALLOC(ap, sizeof(struct aln_param));

        ap->tree = NULL;
        MMALLOC(ap->tree, sizeof(int) * (numseq*3+1));

        for(i = 0;i < (numseq*3+1);i++){
                ap->tree[i] = 0;
        }
        ap->subm = NULL;

        srand48_r(time(NULL), &ap->randBuffer);
        MMALLOC(ap->subm,sizeof (float*) * 32);
        for (i = 32;i--;){
                ap->subm[i] = NULL;
                MMALLOC(ap->subm[i],sizeof(float) * 32);
                for (j = 32;j--;){
                        ap->subm[i][j] = 0.0f;
                }
        }
        RUN(set_subm_gaps(param, ap));

        return ap;
ERROR:
        free_ap(ap);
        return NULL;
}

void free_ap(struct aln_param* ap)
{
        int i;
        if(ap){
                if(ap->subm){
                        for (i = 32;i--;){
                                MFREE(ap->subm[i]);
                        }
                        MFREE(ap->subm);
                }
                if(ap->tree){
                        MFREE(ap->tree);
                }
                MFREE(ap);
        }
}


int set_subm_gaps(struct parameters* param, struct aln_param* ap)
{
        int i,j;
        int m_pos = 0;
        short *matrix_pointer = 0;
        short blosum50mt[]={
                5,
                -2,  5,
                -1, -3, 13,
                -2,  5, -4,  8,
                -1,  1, -3,  2,  6,
                -3, -4, -2, -5, -3,  8,
                0, -1, -3, -1, -3, -4,  8,
                -2,  0, -3, -1,  0, -1, -2, 10,
                -1, -4, -2, -4, -4,  0, -4, -4,  5,
                -1,  0, -3, -1,  1, -4, -2,  0, -3,  6,
                -2, -4, -2, -4, -3,  1, -4, -3,  2, -3,  5,
                -1, -3, -2, -4, -2,  0, -3, -1,  2, -2,  3,  7,
                -1,  4, -2,  2,  0, -4,  0,  1, -3,  0, -4, -2,  7,
                -1, -2, -4, -1, -1, -4, -2, -2, -3, -1, -4, -3, -2, 10,
                -1,  0, -3,  0,  2, -4, -2,  1, -3,  2, -2,  0,  0, -1,  7,
                -2, -1, -4, -2,  0, -3, -3,  0, -4,  3, -3, -2, -1, -3,  1,  7,
                1,  0, -1,  0, -1, -3,  0, -1, -3,  0, -3, -2,  1, -1,  0, -1,  5,
                0,  0, -1, -1, -1, -2, -2, -2, -1, -1, -1, -1,  0, -1, -1, -1,  2,  5,
                0, -4, -1, -4, -3, -1, -4, -4,  4, -3,  1,  1, -3, -3, -3, -3, -2,  0,  5,
                -3, -5, -5, -5, -3,  1, -3, -3, -3, -3, -2, -1, -4, -4, -1, -3, -4, -3, -3, 15,
                -1, -1, -2, -1, -1, -2, -2, -1, -1, -1, -1, -1, -1, -2, -1, -1, -1,  0, -1, -3, -1,
                -2, -3, -3, -3, -2,  4, -3,  2, -1, -2, -1,  0, -2, -3, -1, -1, -2, -2, -1,  2, -1,  8,
                -1,  2, -3,  1,  5, -4, -2,  0, -3,  1, -3, -1,  0, -1,  4,  0,  0, -1, -3, -2, -1, -2,  5};

        short blosum62mt[]={
                40,
                -20,  40,
                0, -30,  90,
                -20,  40, -30,  60,
                -10,  10, -40,  20,  50,
                -20, -30, -20, -30, -30,  60,
                0, -10, -30, -10, -20, -30,  60,
                -20,  0, -30, -10,  0, -10, -20,  80,
                -10, -30, -10, -30, -30,  0, -40, -30,  40,
                -10,  0, -30, -10,  10, -30, -20, -10, -30,  50,
                -10, -40, -10, -40, -30,  0, -40, -30,  20, -20,  40,
                -10, -30, -10, -30, -20,  0, -30, -20,  10, -10,  20,  50,
                -20,  30, -30,  10,  0, -30,  0,  10, -30,  0, -30, -20,  60,
                -10, -20, -30, -10, -10, -40, -20, -20, -30, -10, -30, -20, -20,  70,
                -10,  0, -30,  0,  20, -30, -20,  0, -30,  10, -20,  0,  0, -10,  50,
                -10, -10, -30, -20,  0, -30, -20,  0, -30,  20, -20, -10,  0, -20,  10,  50,
                10,  0, -10,  0,  0, -20,  0, -10, -20,  0, -20, -10,  10, -10,  0, -10,  40,
                0, -10, -10, -10, -10, -20, -20, -20, -10, -10, -10, -10,  0, -10, -10, -10,  10,  50,
                0, -30, -10, -30, -20, -10, -30, -30,  30, -20,  10,  10, -30, -20, -20, -30, -20,  0,  40,
                -30, -40, -20, -40, -30,  10, -20, -20, -30, -30, -20, -10, -40, -40, -20, -30, -30, -20, -30, 110,
                0, -10, -20, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -20, -10, -10,  0,  0, -10, -20, -10,
                -20, -30, -20, -30, -20,  30, -30,  20, -10, -20, -10, -10, -20, -30, -10, -20, -20, -20, -10,  20, -10,  70,
                -10,  10, -30,  10,  40, -30, -20,  0, -30,  10, -30, -10,  0, -10,  30,  0,  0, -10, -20, -30, -10, -20,  40};

        short gon250mt[]={
                24,
                0,   0,
                5,   0, 115,
                -3,   0, -32,  47,
                0,   0, -30,  27,  36,
                -23,   0,  -8, -45, -39,  70,
                5,   0, -20,   1,  -8, -52,  66,
                -8,   0, -13,   4,   4,  -1, -14,  60,
                -8,   0, -11, -38, -27,  10, -45, -22,  40,
                -4,   0, -28,   5,  12, -33, -11,   6, -21,  32,
                -12,   0, -15, -40, -28,  20, -44, -19,  28, -21,  40,
                -7,   0,  -9, -30, -20,  16, -35, -13,  25, -14,  28,  43,
                -3,   0, -18,  22,   9, -31,   4,  12, -28,   8, -30, -22,  38,
                3,   0, -31,  -7,  -5, -38, -16, -11, -26,  -6, -23, -24,  -9,  76,
                -2,   0, -24,   9,  17, -26, -10,  12, -19,  15, -16, -10,   7,  -2,  27,
                -6,   0, -22,  -3,   4, -32, -10,   6, -24,  27, -22, -17,   3,  -9,  15,  47,
                11,   0,   1,   5,   2, -28,   4,  -2, -18,   1, -21, -14,   9,   4,   2,  -2,  22,
                6,   0,  -5,   0,  -1, -22, -11,  -3,  -6,   1, -13,  -6,   5,   1,   0,  -2,  15,  25,
                1,   0,   0, -29, -19,   1, -33, -20,  31, -17,  18,  16, -22, -18, -15, -20, -10,   0,  34,
                -36,   0, -10, -52, -43,  36, -40,  -8, -18, -35,  -7, -10, -36, -50, -27, -16, -33, -35, -26, 142,
                0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                -22,   0,  -5, -28, -27,  51, -40,  22,  -7, -21,   0,  -2, -14, -31, -17, -18, -19, -19, -11,  41,   0,  78,
                0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0};


        if(param->sub_matrix){
                if(byg_start(param->sub_matrix,"blosum62BLOSUM62") != -1){
                        matrix_pointer = blosum62mt;
                        //m_pos = 0;
                        //for (i = 0;i < 23;i++){
                        //	for (j = 0;j <= i;j++){
                        //		matrix_pointer[m_pos] = matrix_pointer[m_pos] * 10;
                        //		m_pos++;
                        //	}
                        //}
                        ap->gpo = 55;
                        ap->gpe = 8;
                        ap->tgpe = 1;
                }
                if(byg_start(param->sub_matrix,"blosum50BLOSUM50") != -1){
                        matrix_pointer = blosum50mt;
                        m_pos = 0;
                        for (i = 0;i < 23;i++){
                                for (j = 0;j <= i;j++){
                                        matrix_pointer[m_pos] = matrix_pointer[m_pos] * 10;
                                        m_pos++;
                                }
                        }
                        ap->gpo = 55;
                        ap->gpe = 8;
                        ap->tgpe = 1;
                }
                //vogt....

        }else{
                //LOG_MSG("DNA>>>>> %d", param->dna);
                if(!param->dna){
                        // gpo:5.494941        gpe:0.852492        tgpe:0.442410       bonus: 3.408872     z-cutoff: 58.823309 -> 0.829257 accuracy on bb3
                        ap->gpo = 54.94941;
                        ap->gpe = 8.52492;
                        ap->tgpe = 4.42410;

                        //gpo = 54;
                        //gpe = 8;
                        //tgpe = 4;
                        //-gpo 10.9898        -gpe 0.852492      -tgpe  0.442410    -bonus    0.2   -zcutoff     58.823309
                        //	param->secret = 0.2;
                        matrix_pointer = gon250mt;
                }else{
                        //gpo = 400;
                        //	gpe =  30;
                        //tgpe = 30;

                        //param->gpo = 43.4;
                        //param->gpe = 3.94;
                        //param->tgpe = 29.26;

                        //gpo = 43.4 *5;
                        ap->gpo = 217;
                        ap->gpe = 39.4;
                        ap->tgpe =  292.6;
                        //param->secret = 28.3;
                        param->zlevel = 61.08;
                        param->internal_gap_weight = 49.14;

                }
        }
        if(param->gpo!= -1){
                //param->gpo *= 5;
                ap->gpo = param->gpo;
        }
        if(param->gpe != -1){
                //param->gpe *= 10;
                ap->gpe = param->gpe;
        }
        if(param->tgpe != -1){
                //param->tgpe *= 10;
                ap->tgpe = param->tgpe;
        }

//	if(param->secret != -1){
//		//param->secret *= 10;
//	}else{
        if(param->secret == -1){
                if(!param->dna){
                        param->secret = 0.2;
                }else{
                        param->secret = 283.0;
                }
        }


        //fprintf(stderr,"%d	%d	%d	%d\n",gpo,gpe,tgpe,	 (int)param->secret);

        for (i = 32;i--;){

                for (j = 32;j--;){
                        ap->subm[i][j] = param->secret;//0;//gpe << 1;//-5;// better on Balibase
                }
        }
        if(param->dna){
                /*subm[0][0] += 10;
                  subm[0][1] += 6;
                  subm[1][0] += 6;
                  subm[1][1] += 10;
                  subm[2][2] += 10;
                  subm[2][3] += 6;
                  subm[3][2] += 6;
                  subm[3][3] += 10;*/
//		     A    C    G    T    .    N
//	A   91 -114  -31 -123    0  -43
                ap->subm[0][0] += 91;
                ap->subm[0][1] += -114;
                ap->subm[0][2] += -31;
                ap->subm[0][3] += -123;

//	C -114  100 -125  -31    0  -43
                ap->subm[1][0] += -114;
                ap->subm[1][1] += 100;
                ap->subm[1][2] += -125;
                ap->subm[1][3] += -31;

//	G  -31 -125  100 -114    0  -43
                ap->subm[2][0] += -31;
                ap->subm[2][1] += -125;
                ap->subm[2][2] += 100;
                ap->subm[2][3] += -114;

//	T -123  -31 -114   91    0  -43
                ap->subm[3][0] += -123;
                ap->subm[3][1] += -31;
                ap->subm[3][2] += -114;
                ap->subm[3][3] += 91;

//	.    0    0    0    0    0    0
//	N  -43  -43  -43  -43    0  -43


                /*for (i = 0; i < 4;i++){
                  for (j = 0;j < 4;j++){
                  if(i == j){
                  subm[i][j] += 1;
                  }else{
                  subm[i][j] -= 3;
                  }
                  }
                  }*/

        }else{

                m_pos = 0;
                for (i = 0;i < 23;i++){
                        for (j = 0;j <= i;j++){
                                if (i == j){
                                        //	subm[i][j] += blosum62mt[m_pos]*10;
                                        ap->subm[i][j] += matrix_pointer[m_pos];
                                }else{
                                        //	subm[i][j] += blosum62mt[m_pos]*10;
                                        //	subm[j][i] += blosum62mt[m_pos]*10;
                                        ap->subm[i][j] += matrix_pointer[m_pos];
                                        ap->subm[j][i] += matrix_pointer[m_pos];
                                }
                                m_pos++;
                        }
                }
                /*for (i = 0; i < 23;i++){
                  for (j = 0; j < 23;j++){
                  fprintf(stderr,"%d ",subm[i][j]);
                  }
                  fprintf(stderr,"\n");
                  }
                  fprintf(stderr,"\n");*/
        }



float balimt[]={
 2.424083,
 0.713758, 5.136856,
 -0.233855, -1.022763, 3.127137,
 0.156475, -1.118003, 1.165819, 2.677455,
 -0.430164, -0.049644, -1.117847, -0.998878, 3.528225,
 0.231454, -0.661056, -0.162995, -0.477668, -1.120880, 3.252141,
 -0.289425, -0.465676, 0.174869, 0.140145, 0.217251, -0.584056, 4.134759,
 -0.092501, 0.120811, -1.370667, -0.978259, 0.621216, -1.507104, -0.738560, 2.841549,
 0.178067, -1.008793, 0.303765, 0.839883, -0.952010, -0.316084, 0.270211, -0.772324, 2.625250,
 -0.157522, 0.007387, -1.173251, -0.822087, 1.024751, -1.367785, -0.354758, 1.377491, -0.547317, 2.525670,
 0.072540, 0.322962, -0.944500, -0.501223, 0.896519, -1.024390, -0.227473, 0.977300, -0.251151, 1.464410, 3.624318,
 -0.106527, -0.353106, 1.042361, 0.335854, -0.692264, 0.297871, 0.774356, -1.017318, 0.444206, -0.693763, -0.412124, 3.139576,
 -0.047197, -0.580472, -0.142603, -0.014469, -0.617813, -0.434219, -0.381892, -0.768431, -0.103144, -0.696139, -0.701903, -0.201856, 3.640785,
 0.137375, -0.651754, 0.428937, 1.029683, -0.575727, -0.466620, 0.642587, -0.632542, 0.976909, -0.262418, 0.195401, 0.461718, -0.194037, 2.956101,
 -0.160676, -0.620646, -0.202300, 0.274883, -0.809382, -0.706284, 0.497874, -0.697052, 1.299535, -0.484351, -0.271819, 0.200629, -0.391753, 0.777804, 3.245137,
 0.754653, 0.207773, 0.297131, 0.178496, -0.641336, 0.155125, -0.019988, -0.806684, 0.157170, -0.745921, -0.298969, 0.603932, 0.086865, 0.215995, -0.055781, 2.485773,
 0.312137, 0.377148, -0.060139, 0.036700, -0.378580, -0.460536, -0.063786, -0.029160, 0.156104, -0.244012, 0.100665, 0.290268, -0.221291, 0.162184, -0.036606, 1.074394, 2.847657,
 0.386519, 0.678010, -1.196287, -0.759928, 0.322343, -1.327412, -0.537227, 1.829940, -0.521054, 0.871494, 0.616202, -0.808055, -0.498073, -0.452713, -0.589252, -0.489048, 0.447141, 2.640269,
 -0.618667, -0.576851, -0.875822, -0.858012, 1.457666, -1.058431, 0.475749, -0.215472, -0.819551, 0.083969, 0.273321, -0.616866, -0.924414, -0.445784, -0.511095, -0.520417, -0.533024, -0.343491, 5.469367,
 -0.377189, -0.291204, -0.757180, -0.717054, 1.957742, -1.010282, 1.030835, -0.097804, -0.521623, 0.249237, 0.281573, -0.424545, -0.742926, -0.219487, -0.201875, -0.452123, -0.359910, -0.152964, 1.681790, 3.863549,
 0.406005, 0.224389, 1.178370, 1.139295, 0.293945, -0.500232, -0.696111, 0.643873, 0.647369, 0.454195, 1.105162, 0.772580, -1.127361, 1.863570, -0.679895, 0.634026, 0.314431, 0.338263, -1.049780, -0.392205, 6.988003,
 4.276194, 6.914756, 4.852787, 4.576633, 5.401664, 4.392723, 6.124841, 4.777292, 4.806955, 4.183224, 6.235049, 5.324184, 5.154922, 5.553503, 5.015684, 4.817209, 4.973070, 4.557893, 7.055625, 5.632280, 12.925539, 28.316347,
 4.276194, 6.914756, 4.852787, 4.576633, 5.401664, 4.392723, 6.124841, 4.777292, 4.806955, 4.183224, 6.235049, 5.324184, 5.154922, 5.553503, 5.015684, 4.817209, 4.973070, 4.557893, 7.055625, 5.632280, 12.925539, 28.316347, 28.316347,
};

        ap->gpo = 8.304180 / 2.0;
ap->gpe =  0.139298;
ap->tgpe = 0.0;

        m_pos = 0;
        for (i = 0;i < 23;i++){
                for (j = 0;j <= i;j++){
                        if (i == j){
                                //	subm[i][j] += blosum62mt[m_pos]*10;
                                ap->subm[i][j] = balimt[m_pos]*2;
                        }else{
                                //	subm[i][j] += blosum62mt[m_pos]*10;
                                //	subm[j][i] += blosum62mt[m_pos]*10;
                                ap->subm[i][j] = balimt[m_pos]*2;
                                ap->subm[j][i] = balimt[m_pos]*2;
                        }
                        m_pos++;
                }
        }



        return OK;
}
