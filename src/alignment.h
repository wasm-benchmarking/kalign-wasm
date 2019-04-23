#ifndef ALIGNMENT_H
#define ALIGNMENT_H

#include "global.h"
#include "alignment_parameters.h"
#include <float.h>


extern int** hirschberg_alignment(struct alignment* aln, struct aln_param* ap);

#endif