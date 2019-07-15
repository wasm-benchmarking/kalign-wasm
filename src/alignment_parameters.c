
#include "alignment_parameters.h"

#include "alphabet.h"
int set_subm_gaps(struct aln_param* ap);
int set_subm_gaps_DNA(struct aln_param* ap);
int set_param_number(struct aln_param* ap,int L, int sel);
int read_aln_param_from_file(struct aln_param* ap, char* infile, int L);

struct aln_param* init_ap(int numseq,int L)
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

        RUNP(ap->rng = init_rng(42));
        //srand48_r(time(NULL), &ap->randBuffer);
        MMALLOC(ap->subm,sizeof (float*) * 32);
        for (i = 32;i--;){
                ap->subm[i] = NULL;
                MMALLOC(ap->subm[i],sizeof(float) * 32);
                for (j = 32;j--;){
                        ap->subm[i][j] = 0.0f;
                }
        }
        //if(param->param_set == -1){

        if(L == defDNA){

                //exit(0);
                RUN(set_subm_gaps_DNA(ap));
        }else if(L == defPROTEIN){

                RUN(set_subm_gaps(ap));
        }else if(L == redPROTEIN){
                RUN(set_subm_gaps(ap));
        }
        //}else{
        //set_param_number(ap, L, param->param_set);
        //}
        /* read parameters from file */
        //if(param->aln_param_file){
        //LOG_MSG("Reading parameters from file: %s", param->aln_param_file);
        //RUN(read_aln_param_from_file(ap, param->aln_param_file, L));
        //}


        return ap;
ERROR:
        free_ap(ap);
        return NULL;
}

int read_aln_param_from_file(struct aln_param* ap, char* infile, int L)
{
        char line_buffer[BUFFER_LEN];
        FILE* f_ptr = NULL;
        double* values = NULL;
        int i,j;
        int m_pos;
        double val = 0;

        int expected;


        expected = (L * (L-1)) / 2 + L + 3;

        MMALLOC(values, sizeof(double) * expected); /* NEED to remove constant!!!  */

        if(!my_file_exists(infile)){
                ERROR_MSG("File: %s does not exist", infile);
        }
        RUNP( f_ptr = fopen(infile, "r"));

        m_pos =0;
        while (fgets(line_buffer , BUFFER_LEN, f_ptr)){
                sscanf(line_buffer, "%lf", &val);
                values[m_pos] = val;
                m_pos++;
                //fprintf(stdout,"%s", ret);
        }

        //fprintf(stdout,"-%s-", ret);


        if(expected != m_pos){
                ERROR_MSG("Expected number of parameters (%d) do not match number of parameters in file %s (%d)", expected,infile,m_pos);
        }

        m_pos = 0;
        for (i = 0;i < L;i++){
                for (j = 0;j <= i;j++){

                        ap->subm[i][j] = values[m_pos];
                        ap->subm[j][i] = ap->subm[i][j];
                        m_pos++;
                }
        }

        ap->gpo = values[m_pos];
        m_pos++;
        ap->gpe = values[m_pos];
        m_pos++;

        ap->tgpe = values[m_pos];

        MFREE(values);
        fclose(f_ptr);

        return OK;
ERROR:
        return FAIL;
}

int set_param_number(struct aln_param* ap,int L, int sel)
{
        int i,j,m_pos;
        double* ptr = NULL;

        double defprot_set10[4][234] = {{ 2.441135 ,0.804586,5.229447,-0.275312,-0.932531,3.078253,0.117964,-1.112335,1.203915,2.665036,-0.478196,0.062497,-1.199822,-1.106372,3.544699,0.243667,-0.629188,-0.166715,-0.522073,-1.419407,3.295848,-0.252812,-0.304885,0.225093,0.189015,0.248641,-0.700361,4.058245,-0.116301,0.221291,-1.419282,-1.003624,0.588717,-1.604453,-0.759925,2.735987,0.182665,-0.915737,0.321957,0.857994,-1.022471,-0.334935,0.297575,-0.784649,2.621895,-0.172126,0.172567,-1.153285,-0.827709,1.022751,-1.503650,-0.309763,1.354375,-0.528795,2.504544,0.090768,0.449192,-0.994454,-0.448665,0.881127,-1.049855,-0.151963,0.958608,-0.219735,1.535655,3.595027,-0.078957,-0.296744,1.113714,0.378894,-0.733749,0.377618,0.809122,-1.043225,0.560572,-0.659316,-0.266522,3.114087,0.077683,-0.717091,-0.075007,-0.003924,-0.752411,-0.544078,-0.270432,-0.714819,-0.032959,-0.715190,-0.657222,-0.232401,3.808283,0.209099,-0.576439,0.480587,1.082240,-0.622755,-0.442365,0.731248,-0.597657,1.065589,-0.192281,0.282779,0.572326,-0.163616,2.996754,-0.216587,-0.556078,-0.276231,0.165220,-0.918021,-0.771466,0.554353,-0.743857,1.327472,-0.543696,-0.286912,0.152924,-0.544813,0.760249,3.307940,0.862154,0.406596,0.359384,0.221433,-0.719529,0.199665,0.082347,-0.820916,0.216351,-0.743032,-0.184038,0.754032,0.186512,0.303803,-0.080536,2.664838,0.340064,0.423405,-0.046178,0.011205,-0.507827,-0.516927,-0.055173,-0.010057,0.160184,-0.246784,0.196552,0.363234,-0.210271,0.156528,-0.168975,1.174802,2.941623,0.405328,0.778105,-1.278921,-0.843631,0.290444,-1.474030,-0.519590,1.808021,-0.548076,0.844055,0.587147,-0.776456,-0.532748,-0.462248,-0.630362,-0.455832,0.518245,2.608431,-0.514309,-0.347037,-1.197980,-0.918742,1.470235,-1.258146,0.590844,-0.170494,-0.813930,0.109186,0.385784,-0.639783,-0.857979,-0.484561,-0.631388,-0.601573,-0.599450,-0.220994,5.505481,-0.357527,-0.191207,-0.925302,-0.866943,1.973688,-1.203284,1.101391,-0.080217,-0.573368,0.265681,0.335302,-0.388156,-0.855382,-0.290231,-0.315724,-0.517638,-0.432202,-0.138017,1.636025,3.872373,1.681627,0.937222,2.171531,-0.001955,-0.764385,-0.115077,-0.025456,0.620326,2.017909,0.064413,0.122257,0.283459,2.263544,0.488142,0.407935,0.793370,0.866565,0.434452,0.959704,-0.491955,15.191782,3.610458,0.378310,0.006965},
                                        { 2.375830 ,0.643607,5.204934,-0.249461,-1.173050,3.172340,0.192837,-1.161841,1.093372,2.710988,-0.401979,-0.156276,-1.082798,-0.903211,3.434700,0.146163,-0.753883,-0.281686,-0.506025,-0.867086,3.140448,-0.341275,-0.662201,0.089096,0.085106,0.143585,-0.504392,4.277457,-0.160461,-0.027477,-1.482790,-1.091093,0.585205,-1.579484,-0.771554,2.841786,0.153642,-1.213189,0.278857,0.860792,-0.897669,-0.341059,0.280853,-0.865861,2.698454,-0.171662,-0.185907,-1.303096,-0.882391,1.030682,-1.331878,-0.406372,1.341062,-0.610478,2.549526,0.074865,0.259681,-0.892696,-0.553258,1.001488,-1.028818,-0.273395,0.982579,-0.227282,1.441501,3.773014,-0.155071,-0.360285,0.950083,0.303635,-0.666252,0.153988,0.769154,-1.107452,0.331698,-0.746403,-0.521854,3.255696,-0.165589,-0.340985,-0.185337,0.059935,-0.340691,-0.326977,-0.467090,-0.829518,-0.133347,-0.614695,-0.668856,-0.099074,3.706510,0.103590,-0.720914,0.413633,1.047622,-0.446286,-0.538221,0.626237,-0.709933,0.965443,-0.296786,0.237723,0.417615,-0.120407,3.110847,-0.088261,-0.704532,-0.124559,0.455879,-0.703728,-0.701110,0.466465,-0.718099,1.318186,-0.419700,-0.184599,0.296735,-0.130674,0.927516,3.208160,0.758824,0.095418,0.309572,0.255178,-0.477662,0.166401,-0.056159,-0.792113,0.209069,-0.715987,-0.306894,0.537011,0.108416,0.257043,0.085939,2.590287,0.298809,0.438208,-0.109154,0.072396,-0.189854,-0.434899,-0.020938,-0.117846,0.157773,-0.237370,0.033414,0.220778,-0.150298,0.249437,0.158230,1.121004,2.827351,0.304373,0.589297,-1.217991,-0.745985,0.318079,-1.316178,-0.573843,1.760337,-0.547804,0.877044,0.668480,-0.902387,-0.401172,-0.437892,-0.580874,-0.477051,0.369209,2.616250,-0.735670,-0.888142,-0.490997,-0.757002,1.491052,-0.854143,0.408417,-0.289839,-0.822824,0.084759,0.237157,-0.467658,-0.908696,-0.278850,-0.281685,-0.225505,-0.363700,-0.517814,5.404440,-0.445265,-0.443508,-0.624691,-0.558171,1.873487,-0.883587,0.883846,-0.224340,-0.458380,0.207657,0.276651,-0.487803,-0.547611,-0.040354,-0.023209,-0.286858,-0.255100,-0.244120,1.783873,3.796554,0.284200,0.077413,1.186291,1.219788,0.347297,-0.586510,-0.620932,0.601297,0.662807,0.455680,1.132707,0.727649,-1.095943,1.930136,-0.521231,0.693074,0.349914,0.299677,-1.077936,-0.408848,5.696912,3.555584,0.356512,0.005053},
                                        { 2.988765 ,0.330658,6.048533,-0.372999,-1.742733,3.355200,-0.111936,-1.441599,1.001284,3.255966,-1.296498,-0.009322,-2.206448,-2.124382,3.641940,0.220432,-1.208191,-0.136338,-0.700479,-1.834093,3.354263,-0.806861,-0.518831,-0.391664,-0.448703,-0.138816,-1.000365,4.531540,-0.458200,-0.773621,-1.836550,-1.328077,0.249780,-1.869329,-1.983229,3.246271,0.098474,-0.668583,-0.220930,0.268151,-2.047742,-0.625460,-0.453621,-1.241727,3.605983,-0.828022,-0.213279,-2.056982,-1.590836,0.527728,-2.047133,-1.877396,1.103220,-1.258418,2.986776,-0.723798,-0.379368,-1.803284,-1.323239,0.214706,-1.797275,-1.082570,0.781156,-0.807643,1.257172,4.823113,-0.306136,-0.774508,0.994684,-0.109040,-1.280948,0.251180,0.419551,-0.988364,0.090954,-1.389571,-1.361113,2.994998,0.142169,-0.165729,-0.637585,-0.797158,-1.731014,-0.608888,-1.184747,-1.369715,-0.407386,-1.399763,-1.536961,-0.516908,4.004747,-0.114701,-0.494646,0.018146,0.971865,-1.456246,-0.569627,0.213520,-1.029848,1.136034,-0.937544,-0.255363,0.265655,-0.326582,3.448932,-0.251985,-0.585584,-0.438468,-0.072998,-1.351157,-0.826051,-0.161218,-1.252762,1.447143,-0.941115,-1.055739,0.048983,-0.655716,0.888946,3.372770,0.693722,0.213017,0.124399,-0.253277,-1.099570,0.154263,-0.763192,-0.919637,-0.284745,-1.078317,-0.839221,0.758804,0.096349,0.097942,-0.140681,2.760864,0.074674,-0.733432,-0.390260,-0.161833,-1.140534,-0.969146,-1.347766,-0.015379,0.093481,-0.775850,-0.118827,0.250024,-0.459992,0.046860,-0.033931,1.018917,3.083992,0.335871,0.106340,-1.755562,-1.113193,-0.327028,-1.692354,-1.970581,1.840384,-0.995567,0.403277,0.473676,-1.380651,-0.847685,-0.969741,-1.183394,-0.948561,0.127447,3.248558,-1.677426,-1.771605,-2.385338,-1.744890,0.198857,-2.256367,-0.465483,-1.811524,-2.492853,-1.188690,-0.640855,-2.791854,-1.573229,-1.866240,-1.559244,-1.814671,-1.882153,-1.824870,5.387924,-1.161668,-0.657621,-1.193655,-1.380692,1.584365,-1.809631,0.788082,-1.233994,-1.582458,-0.374156,-0.552714,-1.283069,-1.770388,-1.336656,-0.976387,-1.070930,-1.387361,-1.263470,-0.092070,3.852683,4.438058,7.271657,4.565167,4.479790,4.709276,4.265295,5.534752,4.712102,5.107466,3.930502,6.200284,4.618413,4.974178,5.182669,4.633832,4.421237,4.559339,4.531892,5.911335,4.786914,21.954597,3.794700,0.421863,0.027185},
                                        { 2.232931 ,1.057775,3.872678,0.169553,-0.103676,2.758993,0.369081,-0.135230,1.157821,2.383283,-0.102529,0.258417,-0.662975,-0.612489,3.413148,0.481787,0.212790,0.371571,-0.012080,-0.633158,3.039450,0.034057,0.225904,0.338794,0.225409,0.327077,-0.080396,3.825504,0.207216,0.644692,-0.632450,-0.372329,0.666708,-0.789520,-0.390353,2.659703,0.465312,0.091239,0.445652,0.797031,-0.503043,-0.014689,0.370923,-0.278195,2.259899,0.170175,0.572394,-0.537310,-0.297406,0.962193,-0.658364,-0.099282,1.418371,-0.142005,2.326511,0.519208,0.911975,-0.348098,-0.080380,0.760070,-0.352575,0.063337,1.177040,0.011250,1.395354,3.286165,0.249498,0.165238,1.087545,0.549153,-0.283970,0.578362,0.925880,-0.436958,0.606512,-0.314716,-0.114963,2.672350,0.409597,-0.035851,0.244759,0.313980,-0.474016,0.216863,0.023327,-0.386687,0.289280,-0.260558,-0.101728,0.311883,3.074851,0.426409,0.289590,0.647739,1.076847,-0.346301,0.161717,0.712282,-0.186318,0.991033,0.004614,0.271507,0.602110,0.276272,2.471184,0.182095,0.250206,0.127889,0.447480,-0.349322,-0.161525,0.481948,-0.249548,1.252403,-0.111172,0.079122,0.464390,0.106980,0.720093,2.869985,0.791158,0.874919,0.556542,0.393894,-0.239243,0.466838,0.444613,-0.230905,0.448122,-0.110508,0.187499,0.772252,0.578583,0.637750,0.358795,1.963403,0.524514,0.661450,0.403349,0.426756,-0.144736,0.083357,0.284615,0.237865,0.572066,0.139552,0.428595,0.585936,0.263856,0.524917,0.371733,1.144194,2.356782,0.596024,0.988593,-0.478686,-0.156852,0.535219,-0.513215,-0.227649,1.806518,-0.049600,0.986069,0.830472,-0.269083,-0.181403,0.058759,-0.143302,0.026346,0.457791,2.430302,-0.376166,0.414717,-0.715912,-0.599316,1.426047,-0.699126,0.215677,-0.055418,-0.275381,0.298257,0.272396,-0.601420,-0.669864,-0.210639,-0.313587,-0.512138,-0.328567,-0.068667,5.370957,-0.029636,0.429613,-0.408193,-0.443011,2.040433,-0.524910,1.127206,0.219616,-0.277388,0.324859,0.266845,-0.083663,-0.397247,-0.171474,-0.114692,-0.151616,-0.074518,0.231096,1.860899,3.592874,0.442121,1.658955,0.638475,0.303002,1.696762,-0.132350,0.415602,1.117820,0.167463,0.921516,2.669780,0.844726,0.133353,-0.747866,-0.440784,0.428838,-0.201140,0.162823,1.468113,1.364092,6.001722,3.747994,0.313172,-0.001827}
        };
        ptr = defprot_set10[sel];
        m_pos = 0;
        for (i = 0;i < L;i++){
                for (j = 0;j <= i;j++){

                        ap->subm[i][j] = ptr[m_pos];
                        ap->subm[j][i] = ap->subm[i][j];
                        m_pos++;
                }
        }

        ap->gpo = ptr[m_pos];
        m_pos++;
        ap->gpe = ptr[m_pos];
        m_pos++;

        ap->tgpe = ptr[m_pos];

        return OK;
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
                if(ap->rng){
                        free_rng(ap->rng);
                }
                if(ap->tree){
                        MFREE(ap->tree);
                }
                MFREE(ap);
        }
}


int set_subm_gaps_DNA(struct aln_param* ap)
{
        int i,j;


        for(i = 0; i < 5; i++){

                for(j =0; j < 5;j++){
                        ap->subm[i][j] = 283;
                }
        }
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

        ap->gpo = 217;
        ap->gpe = 39.4;
        ap->tgpe =  292.6;
        //param->secret = 28.3;
 			  //		     A    C    G    T    .    N

        return OK;
}

int set_subm_gaps(struct aln_param* ap)
{
        int i,j;
        int m_pos = 0;
        float *matrix_pointer = 0;
        float gon250mt[]={
                24,
                5,    115,
                -3,    -32,  47,
                0,   -30,  27,  36,
                -23,   -8, -45, -39,  70,
                5,    -20,   1,  -8, -52,  66,
                -8,   -13,   4,   4,  -1, -14,  60,
                -8,   -11, -38, -27,  10, -45, -22,  40,
                -4,   -28,   5,  12, -33, -11,   6, -21,  32,
                -12,  -15, -40, -28,  20, -44, -19,  28, -21,  40,
                -7,    -9, -30, -20,  16, -35, -13,  25, -14,  28,  43,
                -3,   -18,  22,   9, -31,   4,  12, -28,   8, -30, -22,  38,
                3,   -31,  -7,  -5, -38, -16, -11, -26,  -6, -23, -24,  -9,  76,
                -2,   -24,   9,  17, -26, -10,  12, -19,  15, -16, -10,   7,  -2,  27,
                -6,   -22,  -3,   4, -32, -10,   6, -24,  27, -22, -17,   3,  -9,  15,  47,
                11,     1,   5,   2, -28,   4,  -2, -18,   1, -21, -14,   9,   4,   2,  -2,  22,
                6,    -5,   0,  -1, -22, -11,  -3,  -6,   1, -13,  -6,   5,   1,   0,  -2,  15,  25,
                1,     0, -29, -19,   1, -33, -20,  31, -17,  18,  16, -22, -18, -15, -20, -10,   0,  34,
                -36,  -10, -52, -43,  36, -40,  -8, -18, -35,  -7, -10, -36, -50, -27, -16, -33, -35, -26, 142,
                -22,   -5, -28, -27,  51, -40,  22,  -7, -21,   0,  -2, -14, -31, -17, -18, -19, -19, -11,  41,   78,
                0,     0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0, 0};
        ap->gpo = 54.94941;
        ap->gpe = 8.52492;
        ap->tgpe = 4.42410;


        matrix_pointer = gon250mt;
        ap->gpo = 54.94941;
        ap->gpe = 8.52492;
        ap->tgpe = 4.42410;

        float balimt[]={
                24.501946,
                5.998169, 115.750240,
                -2.470710, -31.062287, 47.937530,
                0.999786, -29.101076, 28.000000, 36.003891,
                -22.005890, -7.007568, -44.750011, -38.000458, 71.000000,
                6.000000, -19.000000, 2.000000, -7.015625, -51.000000, 66.992218,
                -9.000000, -12.000000, 4.843778, 4.934356, -0.000031, -13.499763, 60.750057,
                -7.000855, -10.015595, -37.000214, -26.249912, 10.985351, -44.001923, -21.030732, 40.753445,
                -3.000214, -27.998062, 6.000000, 12.000229, -32.055085, -10.000061, 6.999969, -20.013794, 32.875029,
                -11.007813, -14.000000, -39.000000, -27.124605, 20.844236, -43.003876, -18.001831, 29.000000, -20.000458, 40.875059,
                -6.015106, -8.986221, -29.128878, -19.062470, 16.875029, -34.029297, -12.000946, 25.503868, -13.000000, 29.000000, 43.938384,
                -2.499519, -17.003632, 22.780331, 10.000000, -30.001923, 4.999786, 12.999542, -27.375036, 9.000000, -31.000000, -21.000000, 38.902403,
                3.999908, -30.249973, -6.060548, -4.000000, -37.003662, -15.000000, -10.029297, -25.246525, -5.001801, -22.015595, -23.124971, -8.500008, 77.000000,
                -1.000214, -23.499855, 9.999786, 17.000473, -25.014832, -9.000092, 12.624781, -18.148531, 15.877928, -15.031189, -9.015595, 7.999786, -1.062470, 27.000473,
                -5.001923, -21.078096, -2.124971, 5.000000, -31.750011, -9.000000, 7.000000, -23.030274, 27.999542, -21.492195, -16.001923, 3.757809, -8.000000, 15.500023, 47.984375,
                11.999054, 1.996338, 5.875120, 3.000000, -27.000000, 4.875029, -1.250919, -17.499977, 2.000000, -20.046876, -13.015564, 9.972198, 4.546899, 2.265614, -1.062013, 22.750027,
                6.993225, -4.031220, 1.000000, -0.499977, -21.000214, -10.000000, -2.062013, -5.000946, 1.985351, -12.999985, -5.000000, 6.000000, 1.562402, -0.500481, -1.000519, 15.960937, 25.986114,
                0.001923, 0.554681, -28.999985, -18.999557, 1.968780, -32.124025, -19.031220, 32.000000, -16.999985, 18.750027, 16.500053, -21.875227, -17.000458, -14.499519, -19.124971, -9.499886, 0.000015, 34.999512,
                -35.249973, -9.000031, -51.062959, -42.996109, 36.996124, -39.048310, -7.503426, -17.015595, -34.124971, -7.984436, -9.063233, -35.187503, -49.496101, -26.000214, -15.000092, -32.265599, -34.937026, -25.499977, 143.000000,
                -21.007782, -4.999985, -27.999985, -26.015595, 51.875029, -39.242649, 22.750027, -6.000458, -20.015595, 0.999969, -1.000000, -13.500008, -30.000000, -16.000458, -17.059052, -18.062470, -18.055146, -10.109377, 41.000107, 78.000961,
                0.750973, 0.621088, 1.000000, 0.750027, 0.999786, 0.937530, 0.937560, 0.984405, 0.999054, 0.991241, 1.000000, 0.871580, 0.999786, 0.031235, 1.000000, 0.265614, 0.097642, 0.969726, 0.999054, 1.000000, 0.999908,
        };
        ap->gpo = 55.918190;
        ap->gpe =  9.335495;
        ap->tgpe =  5.017874;

        matrix_pointer = balimt;
        m_pos = 0;

        for (i = 0;i < 21;i++){
                for (j = 0;j <= i;j++){
                        ap->subm[i][j] = matrix_pointer[m_pos];
                        ap->subm[j][i] = matrix_pointer[m_pos];
                        m_pos++;
                }
        }

        return OK;
}
