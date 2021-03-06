/*

Copyright (C) 2003 The GeoFramework Consortium

This file is part of Ellipsis3D.

Ellipsis3D is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License, version 2,
as published by the Free Software Foundation.

Ellipsis3D is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

Authors:
  Louis Moresi <louis.moresi@sci.monash.edu>
  Richard Albert <richard.albert@exxonmobil.com>

*/


/*  */ 
/* This file contains the definitions of variables which are passed as arguments */
/* to functions across the whole filespace of Ellipsis. #include this file everywhere !*/
  
 
/* 
#include <assert.h>  
#include <stdlib.h> 
*/


#include <stdio.h>

void *Malloc1(); 
void Realloc1();

/* #define fft twiddle_thumbs
   #define rft twiddle_thumbs */



#define Malloc0(a) Malloc1((a),__FILE__,__LINE__)
#define Realloc0(a,b,c,d) Realloc1((a),(b),(c),(d),__FILE__,__LINE__)


/* ============================================== */

/*

The following "module" includes were originally

    #include "_options_definitions_files.hh"

where "_options_definitions_files.hh" was generated by a shell script,
roughly as follows:

    for file in *global_definitions.h ; do
        echo "#include \"$file\" " >> _options_definitions_files.hh
    done


*/
#include "tracer_global_definitions.h" 
#include "chemtrans_global_definitions.h" 

/* ============================================== */


/* For 32 bit safe ... must be smaller than 0xFFFFFFFF 
   i.e. stop when you reach 0x100000000 and think about it */

#define SBC4 0x1	/* node has prescribed x couple stress */
#define BC1 0x2		/* node has prescribed x velocity */
#define BC2 0x4		/* node had prescribed z velocity */
#define BC3 0x8		/* node has prescribed y velocity or rotation */
#define TBD 0x10	/* node has prescribed temperature */
#define TZEDGE 0x80	/* node is on a constant z boundary surface (temperature) - not used */
#define TXEDGE 0x100	/* node is on a constant x boundary surface (temperature) - not used */
#define TYEDGE 0x200	/* node is on a constant y boundary surface (temperature) - not used */
#define VXEDGE 0x400	/* node is on a constant x boundary surface (velocity) - not used */
#define VZEDGE 0x800	/* node is on a constant z boundary surface (velocity) - not used */
#define VYEDGE 0x1000	/* node is on a constant y boundary surface (velocity) - not used */
#define INTX 0x2000	/* node has ...(interpolation in x)? - not used */
#define INTZ 0x4000	/* node has ...(interpolation in z)? - not used */
#define INTY 0x8000	/* node has ...(interpolation in y)? - not used */
#define SBC1 0x10000	/* node has prescribed x stress */
#define SBC2 0x20000	/* node has prescribed z stress */
#define SBC3 0x40000	/* node has prescribed y stress or couple stress */
#define FBX 0x80000	/* node has prescribed heat flux in x */
#define FBZ 0x100000	/* node has prescribed heat flux in z */
#define FBY 0x200000	/* node has prescribed heat flux in y */
#define OFFSIDE 0x400000	/* node is periodic in x */
#define SBC5 0x800000		/* node has prescribed z couple stress */
#define SKEWBC 0x1000000	/* node has skewed BCs */
#define PER_OFFSIDE 0x2000000	/* node is periodic */
#define BC4 0x4000000		/* node has prescribed x rotation */
#define BC5 0x8000000		/* node has prescribed z rotation */
#define BC6 0x10000000		/* node has prescribed y rotation */
#define SBC6 0x20000000		/* node has prescribed y couple stress */

/* DAS: this is now an option in the input template
#ifndef COMPRESS_BINARY
#define COMPRESS_BINARY "/usr/local/bin/gzip"
#endif
*/

#define MAX_LEVELS 12	/* maximum number of multigrid levels */
#define MAX_DOF 7	/* maximum number of degrees of freedom available */
#define MAX_LOAD_SEQS 2   /*RAA, 19/02/02 max. # of loading sequences*/


#define VE_TIMETOLERANCE 2.5



/* Macros */

#define max(A,B) (((A) > (B)) ? (A) : (B))
#define min(A,B) (((A) < (B)) ? (A) : (B))
#define SWAP(a,b) {temp=(a);(a)=(b);(b)=temp;}

typedef double standard_precision;
typedef double higher_precision;	/* matrix coeffs etc */
typedef double highest_precision;	/* intermediate calculations for finding above coeffs */






/* Common structures */

struct MeshRegion {
    int numb;
    standard_precision offset[40];
    standard_precision compression[40];
    standard_precision width[40];
    standard_precision edge_width[40];
};

struct Rect {
    int numb;
    char overlay[40];
    standard_precision x1[40];
    standard_precision x2[40];
    standard_precision z1[40];
    standard_precision z2[40];
    standard_precision y1[40];
    standard_precision y2[40];
    standard_precision halfw[40];
    standard_precision mag[40];
} ;
 

struct Circ {
    int numb;
    char overlay[40];
    standard_precision x[40];
    standard_precision z[40];
    standard_precision y[40];
    standard_precision rad[40];
    standard_precision mag[40];
    standard_precision halfw[40];
};


struct Harm {
    int numb;
    int harms;
    char overlay[40];
    standard_precision off[40];
    standard_precision x1[40];
    standard_precision x2[40];
    standard_precision z1[40];
    standard_precision z2[40];
    standard_precision y1[40];
    standard_precision y2[40];
    standard_precision kx[20][40];
    standard_precision kz[20][40];
    standard_precision ky[20][40];
    standard_precision ka[20][40];
    standard_precision phx[20][40];
    standard_precision phz[20][40];
    standard_precision phy[20][40];
};

struct Erfc {
 int numb;
};

struct RectBc {
    int numb;
    char norm[40];
    standard_precision intercept[40];
    standard_precision aa1[40];
    standard_precision aa2[40];
    standard_precision bb1[40];
    standard_precision bb2[40];
    standard_precision halfw[40];
    standard_precision mag[40];
} ;
 

struct CircBc {
    int numb;
    char norm[40];
    standard_precision intercept[40];
    standard_precision aa[40];
    standard_precision bb[40];
    standard_precision rad[40];
    standard_precision mag[40];
    standard_precision halfw[40];
};


struct PolyBc {
    /*RAA: eventually, there should be a new group of coefficients when the order > 1,
     say, acc[][], for cross-terms, e.g., acc[][]*x*y, etc */
    int numb;
    int order;
    char norm[40];
    standard_precision intercept[40];
    standard_precision aa1[40];
    standard_precision aa2[40];
    standard_precision bb1[40];
    standard_precision bb2[40];
    standard_precision aaa[20][40];
    standard_precision abb[20][40];
};


struct HarmBc {
    int numb;
    int harms;
    char norm[40];
    standard_precision off[40];
    standard_precision intercept[40];
    standard_precision aa1[40];
    standard_precision aa2[40];
    standard_precision bb1[40];
    standard_precision bb2[40];
    standard_precision kaa[20][40];
    standard_precision kbb[20][40];
    standard_precision amp[20][40];
    standard_precision phaa[20][40];
    standard_precision phbb[20][40];
  };

struct Output_and_input {
  int numb;
  int numb_found;
  char which_data_types[1000];     /* List of variables which are to be dumped out */
  char which_data_types_in[1000];  /* ... and read in. Being minimalist, anything
				      which is difficult or impossible to compute from
				      the present state ought to be able to be read
				      back in. If not, does it really need to be printed out ? */
  char name[50][5];
  standard_precision *data[50];
};

struct Shape_function_dA  {
  standard_precision vpt[9];
  standard_precision ppt[2]; };

struct Shape_function1_dA  {
  standard_precision vpt[6*4];
  standard_precision ppt[6*1]; };

struct Shape_function1 	{ 
    standard_precision vpt[4*4];  /* node & gauss pt */
    standard_precision ppt[4*1];  };

struct Shape_function 	{ 
  standard_precision vpt[8*8];  /* node & gauss pt */
  standard_precision ppt[8*1];  };

struct Shape_function_dx 	{ 
  standard_precision vpt[3*8*8]; /* dirn & node & gauss pt */
  standard_precision ppt[3*8*1];  };

struct Shape_function1_dx 	{ 
    standard_precision vpt[2*4*4]; /* dirn & node & gauss pt */
    standard_precision ppt[2*4*1];  };


struct EG { 
    higher_precision g[24][1]; };

struct EK { 
    higher_precision k[24*24]; };

struct MEK { 
    higher_precision nint[9]; };
 
struct NK {
    higher_precision *k;
    int *map;
};

struct CURVILINEAR {
  higher_precision **NODE_R[MAX_LEVELS];
  higher_precision **node_r;
  standard_precision *cost[MAX_LEVELS];
  standard_precision *sint[MAX_LEVELS];
  standard_precision *cosph[MAX_LEVELS];
  standard_precision *sinph[MAX_LEVELS];
};


struct COORD {
  standard_precision centre[4];
  standard_precision size[4];     /* in xzy coords */
  standard_precision recip_size[4];
  standard_precision ntl_size[4]; /* in natural coords */
  standard_precision ntl_recip_size[4];
  standard_precision ntl_dirns[4][4];
  standard_precision area;
   } ;


struct SUBEL { 
  int sub[9];
   };
			
struct ID  { 
    int doff[7];
	};
struct IEN {
    int node[9];
	};
struct LM  { 
    struct {
      int doff[4];
    }
 node[9];
 } ;

struct NEI {
    int *nels;
    int *lnode;
    int *element; };

struct MESH_DATA {/* general information concerning the fe mesh */ 
  int nsd;        /* spatial extent 1,2,3d */
  int dof;        /* degrees of freedom per node */
  int levmax;		/* finest FE grid */
  int levmin;		/* coarsest FE grid */
  int levels;		/* number of multigrid levels */
  int q_levels;		/* number of multigrid levels */
  int q_levmin;		/* coarsest solid pressure FE grid */
  int mgunitx;		/* number of elements in x in coarsest grid */
  int mgunitz;
  int mgunity;
  int NEQ[MAX_LEVELS];	/* All other values refer to the biggest mesh (& lid)  */
  int NNO[MAX_LEVELS];
  int NLNO[MAX_LEVELS];
  int NPNO[MAX_LEVELS];
  int NEL[MAX_LEVELS];
  int NOX[MAX_LEVELS];
  int NOZ[MAX_LEVELS];
  int NOY[MAX_LEVELS];
  int NMX[MAX_LEVELS];
  int ELX[MAX_LEVELS];
  int ELZ[MAX_LEVELS];
  int ELY[MAX_LEVELS];
  int LNDS[MAX_LEVELS];
  int LELS[MAX_LEVELS];
  int neqd;
  int neq;	/* number of equations in finest mesh solution */
  int nno;	/* number of nodes in finest mesh */
  int npno;	/* number of solid pressure nodes in finest mesh */
  int nel;	/* number of elements in finest mesh */
  int nex[4]; /* general form of ... */
  int elx;	/* number of elements in x in finest mesh */
  int elz;
  int ely;
  int nnx[4]; /* general form of ... */
  int nox;	/* number of nodes in x in finest mesh */
  int noz;
  int noy;
  int nmx;
  int nsf; /* nodes for surface observables */

  char topvbc_file[100];
  char botvbc_file[100];
  char sidevbc_file[100];
  char gridfile[4][100];

  struct RectBc Vxrectbcs;
  struct RectBc Vyrectbcs;
  struct RectBc Vzrectbcs;
  struct RectBc Stxrectbcs;
  struct RectBc Styrectbcs;
  struct RectBc Stzrectbcs;
   
  struct CircBc Vxcircbcs;
  struct CircBc Vzcircbcs;
  struct CircBc Vycircbcs;
  struct CircBc Stxcircbcs;
  struct CircBc Stzcircbcs;
  struct CircBc Stycircbcs;

  struct PolyBc Vxpolybcs;
  struct PolyBc Vzpolybcs;
  struct PolyBc Vypolybcs;
  struct PolyBc Stxpolybcs;
  struct PolyBc Stzpolybcs;
  struct PolyBc Stypolybcs;

  struct HarmBc Vxharmbcs;
  struct HarmBc Vzharmbcs;
  struct HarmBc Vyharmbcs;
  struct HarmBc Stxharmbcs;
  struct HarmBc Stzharmbcs;
  struct HarmBc Styharmbcs;
 
  int surface_initial_isostasy;
  int Free_upper_surface;
  int Free_lower_surface;
  int equilibrium_upper_surface;
  int equilibrium_lower_surface;

  struct MeshRegion spacing[3];

  int periodic_x;	/* indicates periodicity at x boundaries */
  int periodic_y;
  int periodic_z;
  int periodic_rm_vx;
  int periodic_rm_vy;   /*RAA: 9/10/01, added this line*/
  standard_precision layer0[4];			/* dimensionless dimensions */
  standard_precision layer1[4];			/* dimensionless dimensions */
   
  standard_precision BCvelocityX1;
  standard_precision BCvelocityX0;
  standard_precision BCvelocityZ1;
  standard_precision BCvelocityZ0;
  standard_precision BCvelocityY1;
  standard_precision BCvelocityY0;
  standard_precision BCvelocity1X1;
  standard_precision BCvelocity1X0;
  standard_precision BCvelocity1Z1;
  standard_precision BCvelocity1Z0;
  standard_precision BCvelocity1Y1;
  standard_precision BCvelocity1Y0;
  standard_precision BCvelocityomegaX1;
  standard_precision BCvelocityomegaX0;
  standard_precision BCvelocityomegaZ1;
  standard_precision BCvelocityomegaZ0;
  standard_precision BCvelocityomegaY1;
  standard_precision BCvelocityomegaY0;
  standard_precision BCvelocityphaseX1;
  standard_precision BCvelocityphaseX0;
  standard_precision BCvelocityphaseZ1;
  standard_precision BCvelocityphaseZ0;
  standard_precision BCvelocityphaseY1;
  standard_precision BCvelocityphaseY0;
  standard_precision BCvelocitypowerX1;
  standard_precision BCvelocitypowerX0;
  standard_precision BCvelocitypowerZ1;
  standard_precision BCvelocitypowerZ0;
  standard_precision BCvelocitypowerY1;
  standard_precision BCvelocitypowerY0;
  standard_precision BCvelocityaccX1;
  standard_precision BCvelocityaccX0;
  standard_precision BCvelocityaccZ1;
  standard_precision BCvelocityaccZ0;
  standard_precision BCvelocityaccY1;
  standard_precision BCvelocityaccY0;
  int BCvelocity_time_interval; /*RAA, 12/02/02, & is boolean in Instructions.c*/
  int BCvelocity_time_or_inc;       /*RAA, 12/02/02 */
  /*RAA: treat increments as std precision in following lines, for simplicity*/
  /*RAA: this stuff is for sequentially bi-directional loading*/
  standard_precision BCvelocityX0_time[2][4][MAX_LOAD_SEQS]; /*RAA, 19/02/02 */
  standard_precision BCvelocityX0_const[3][MAX_LOAD_SEQS];   /*RAA, 19/02/02 */
  standard_precision BCvelocityX0_slope[3][MAX_LOAD_SEQS];   /*RAA, 19/02/02 */
  standard_precision BCvelocityX1_time[2][4][MAX_LOAD_SEQS]; /*RAA, 19/02/02 */
  standard_precision BCvelocityX1_const[3][MAX_LOAD_SEQS];   /*RAA, 19/02/02 */
  standard_precision BCvelocityX1_slope[3][MAX_LOAD_SEQS];   /*RAA, 19/02/02 */
  standard_precision BCvelocityZ0_time[2][4][MAX_LOAD_SEQS]; /*RAA, 19/02/02 */
  standard_precision BCvelocityZ0_const[3][MAX_LOAD_SEQS];   /*RAA, 19/02/02 */
  standard_precision BCvelocityZ0_slope[3][MAX_LOAD_SEQS];   /*RAA, 19/02/02 */
  standard_precision BCvelocityZ1_time[2][4][MAX_LOAD_SEQS]; /*RAA, 19/02/02 */
  standard_precision BCvelocityZ1_const[3][MAX_LOAD_SEQS];   /*RAA, 19/02/02 */
  standard_precision BCvelocityZ1_slope[3][MAX_LOAD_SEQS];   /*RAA, 19/02/02 */
  standard_precision BCvelocityY0_time[2][4][MAX_LOAD_SEQS]; /*RAA, 19/02/02 */
  standard_precision BCvelocityY0_const[3][MAX_LOAD_SEQS];   /*RAA, 19/02/02 */
  standard_precision BCvelocityY0_slope[3][MAX_LOAD_SEQS];   /*RAA, 19/02/02 */
  standard_precision BCvelocityY1_time[2][4][MAX_LOAD_SEQS]; /*RAA, 19/02/02 */
  standard_precision BCvelocityY1_const[3][MAX_LOAD_SEQS];   /*RAA, 19/02/02 */
  standard_precision BCvelocityY1_slope[3][MAX_LOAD_SEQS];   /*RAA, 19/02/02 */
  /*RAA: the variables above allow trapezoidal loading functions. 
     W/ r to the ...time[][][], 
               1st index: '0' is start, '1' is finish
               2nd index:   '0' is local time t0, '1' is local time t1, 
                                etc, '3' is local time t3 
               3rd index:    the particular load sequence, eg, #1 of 3 total loads, etc.
    w/r to ..const[][] & ...slope[][],
               1st index:  is the section of the trapezoid, '0' is left side, etc
               2nd index:    the particular load sequence, eg,#1 of 3 total loads, etc.
  */


  standard_precision fine_spacing[4];
  int equiv_even_nodes[4];
  int equiv_even_nsf;

  int fnodal_malloc_size;
  int dnodal_malloc_size;
  int bandwidth;
  int null_source;
  int null_sink;
} ; 

struct HAVE {		/* horizontal averages */
  standard_precision *T;
  standard_precision *Vi;
  standard_precision *Poro;
  standard_precision *f;
  standard_precision *F;
  standard_precision *vrms;
  standard_precision *V[4];
  standard_precision *strs;
  standard_precision *buoy;
  standard_precision *Pres;
  standard_precision *Depl; /*RAA: 09/07/03 - from C. O'Neill's melting stuff*/
};

struct SLICE {		/* horizontally sliced data, including topography */
  standard_precision *tpg;
  standard_precision *tpgb;
  standard_precision *tpgk;
  standard_precision *tpgbk;
  standard_precision *grv;
  standard_precision *grvk;
  standard_precision *grvt;
  standard_precision *grvtk;
  standard_precision *grvb;
  standard_precision *grvbk;
  standard_precision *geo;
  standard_precision *geok;
  standard_precision *geob;
  standard_precision *geot;
  standard_precision *geobk;
  standard_precision *geotk;
  standard_precision *shflux;
  standard_precision *bhflux;
  standard_precision *depl; /*RAA: 09/07/03 - from C. O'Neill's melting stuff*/
  standard_precision *melt; /*RAA: 09/07/03 - from C. O'Neill's melting stuff*/
  standard_precision *vxsurf[4];    /* surface velocity vectors */
};


struct TOTAL {
    standard_precision melt_prod;
};

struct MONITOR {
  char node_output[100][6];  /* recording the format of the output data */
  char sobs_output[100][6];  /* recording the format of the output data */
  int node_output_cols;
  int sobs_output_cols;

  int solution_cycles;
  int total_gs_cycles[MAX_LEVELS]; 

  standard_precision  time_scale;
  standard_precision  length_scale;
  standard_precision  viscosity_scale;
  standard_precision  tpgscale;
    
  standard_precision  delta_v_last_soln;
  standard_precision  elapsed_time;
  standard_precision  elapsed_time_vsoln;
  standard_precision  elapsed_time_vsoln1;
  standard_precision  incompressibility;
  standard_precision  vdotv;
  standard_precision  nond_av_heat_fl;  
  standard_precision  nond_av_adv_hfl;  
  standard_precision  cpu_time_elapsed;
  standard_precision  cpu_time_on_vp_it;
  standard_precision  cpu_time_on_forces;
  standard_precision  cpu_time_on_mg_maps;
  standard_precision  tpgkmag;
  standard_precision  grvkmag;
  standard_precision  gtrk;
  standard_precision  gtrfit;
  standard_precision  gtrfit_h;
  standard_precision  gtrfit_l;
   
  standard_precision  Nusselt;
  standard_precision  Vmax;
  standard_precision  Vsrms;
  standard_precision  Vrms;
  standard_precision  Vxrms;
  standard_precision  Vzrms;
  standard_precision  Vyrms;
  standard_precision  Vrms_surface;
  standard_precision  Vrms_base;
  standard_precision  F_surface;
  standard_precision  F_base;
  standard_precision  Frat_surface;
  standard_precision  Frat_base;
  standard_precision  T_interior;
  standard_precision  Sigma_max;
  standard_precision  Sigma_interior;
  standard_precision  Vi_average;
  standard_precision  Vis2;

  standard_precision log_average_visc;
  standard_precision max_visc;
  standard_precision min_visc;

  standard_precision reference_stress;
  standard_precision reference_viscosity;

  standard_precision min_BI[MAX_LEVELS];
  standard_precision max_BI[MAX_LEVELS];

  standard_precision time_of_last_store;
  standard_precision time_of_last_checkpt;
   
};

struct CONTROL {
  int PID;

  char output_written_external_command[500];   /* a unix command to run when output files have been created */
  char ppm_written_external_command[500];   /* a unix command to run when output files have been created */

  int ORTHO,ORTHOZ;   /* indicates levels of mesh symmetry */
  int HAVE_SKEWBCS;
  int outputcase;
  int CURRENT_SKEWED_Vs;
  char B_is_good[MAX_LEVELS];  /* general information controlling program flow */
  char Ahat_is_good[MAX_LEVELS];  /* general information controlling program flow */
  char old_P_file[100];
  char data_file[1000];
  char input_file_name[1000];
  int test ;

  char vector_optimization;

  struct Output_and_input node_data; 
  struct Output_and_input slice_data;
  struct Output_and_input time_data;
  struct Output_and_input haverage_data;
  struct Output_and_input particle_data;
  
  char GEOMETRY[20]; /* one of ... */
  int CONVECTION; 
  int CART2D;
  int CART2pt5D; 
  int CART3D;
  int AXI;
  int CYLINDER;
  int SPHERE;
  int AXISPHERE;

  int B_matrix ;
  int model ;
 
  char SOLVER_TYPE[20]; /* one of ... */
  int CONJ_GRAD;
  int MULTIGRID;

  char NODE_SPACING[3][20]; /* turns into ... */
  int GRID_TYPE[3];

  int COMPRESS;
  char gzip[100];  /* DAS: 17/01/03, path to gzip program */
  int AVS;
  int NODE_BY_NODE;
  int ELEMENT_BY_ELEMENT;

  int ELEMENT_TYPE;
  int ELEMENT_TYPE_P;

  int dfact;
  standard_precision AUG_lagrangian;

  standard_precision secret_case_fl[10] ;
  standard_precision particle_perturb;
  standard_precision director_perturb,director_perturbkx;

  standard_precision TBCtopval;
  standard_precision TBCbotval;

  standard_precision Q0;
  standard_precision Atemp ;
  
  int keep_going;
  
  int vsolver_relaxations;
  int max_vel_iterations;
  int mg_low_relaxations;
  
  int p_iterations;
  
  int mg_cycle;
   
  higher_precision accuracy;
  standard_precision delta_accuracy_factor;
  standard_precision gs_under;

  int total_iteration_cycles;
  int total_v_solver_calls;

  int record_all_until;
  int storage_timesteps;
  int checkpt_timesteps;

  int PPM_height;
  standard_precision PPM_aspect;
  int PPM_files; 
  standard_precision PPM_strain;
  int PPM_surf_plot[40];
  int PPM_coloring[40];
  int PPM_color_auto[40];
  standard_precision PPM_color_min[40];
  standard_precision PPM_color_max[40];
  int PPM_surf_auto[40];
  standard_precision PPM_surf_min[40];
  standard_precision PPM_surf_max[40];
  standard_precision PPM_3D_delt; /*RAA: 23-01-03, added this*/

  standard_precision storage_timing;
  standard_precision checkpt_timing;

  int ELASTICITY;
  int ORTHOTROPY;
  int CHEM_TRANS;

  int deformation;
  int verbose;
  int print_convergence;
 
   /* modules */
  int MELTING_MODULE;
  int CHEMISTRY_MODULE;
  int SHEAR_HEATING;

};

struct DATA {  
  standard_precision grav_acc;   
  standard_precision grav_theta;
};

struct ELEMENT {
  int type_v ;		/* description of element type wrt velocity */
  int type_p ;		/* description of element type wrt solid pressure */
};
	      
struct All_variables {
#include "viscosity_descriptions.h"
#include "temperature_descriptions.h"

/* ============================================== */

/* MODULES */
/*

The following "module" includes were originally

    #include "_options_variables_files.hh"

where "_options_variables_files.hh" was generated by a shell script,
roughly as follows:

    for file in *global_variables.h ; do
        echo "#include \"$file\" " >> _options_variables_files.hh
    done

*/
#include "advection_global_variables.h" 
#include "convection_global_variables.h" 
#include "tracer_global_variables.h" 

/* ============================================== */


  FILE *fp,*fp1,*fp2;

  struct HAVE Have;
  struct TOTAL Total;
  struct MESH_DATA mesh;
  struct CONTROL control;
  struct MONITOR monitor;
  struct DATA data;
  struct SLICE slice;
  struct COORD *eco;
  struct IEN *ien;  
  struct IEN *ienp;  
  struct ID *id;
  struct COORD *ECO[MAX_LEVELS];
  struct IEN *IEN[MAX_LEVELS];  
  struct IEN *IENP[MAX_LEVELS]; 
  struct ID *ID[MAX_LEVELS];
  struct NEI NEI[MAX_LEVELS];
  struct SUBEL *EL[MAX_LEVELS];
  struct EG *elt_del[MAX_LEVELS];
  struct EK *elt_k[MAX_LEVELS];
  struct CURVILINEAR curvilinear;
  struct ELEMENT *elem ;
  struct ELEMENT *ELEM[MAX_LEVELS] ;

  higher_precision *Eqn_k[MAX_LEVELS];  
  higher_precision *Node_k11[MAX_LEVELS];	/* element stiffness matrix */
  higher_precision *Node_k21[MAX_LEVELS];
  higher_precision *Node_k31[MAX_LEVELS];
  higher_precision *Node_k12[MAX_LEVELS];
  higher_precision *Node_k22[MAX_LEVELS];
  higher_precision *Node_k32[MAX_LEVELS];
  higher_precision *Node_k13[MAX_LEVELS];
  higher_precision *Node_k23[MAX_LEVELS];
  higher_precision *Node_k33[MAX_LEVELS];

  higher_precision *Node_k14[MAX_LEVELS];
  higher_precision *Node_k15[MAX_LEVELS];
  higher_precision *Node_k16[MAX_LEVELS];
  higher_precision *Node_k24[MAX_LEVELS];
  higher_precision *Node_k25[MAX_LEVELS]; 
  higher_precision *Node_k26[MAX_LEVELS];
  higher_precision *Node_k34[MAX_LEVELS];
  higher_precision *Node_k35[MAX_LEVELS];
  higher_precision *Node_k36[MAX_LEVELS];

  higher_precision *Node_k41[MAX_LEVELS];
  higher_precision *Node_k42[MAX_LEVELS];
  higher_precision *Node_k43[MAX_LEVELS];
  higher_precision *Node_k44[MAX_LEVELS];
  higher_precision *Node_k45[MAX_LEVELS];
  higher_precision *Node_k46[MAX_LEVELS];
  higher_precision *Node_k51[MAX_LEVELS];
  higher_precision *Node_k52[MAX_LEVELS];
  higher_precision *Node_k53[MAX_LEVELS];
  higher_precision *Node_k54[MAX_LEVELS];
  higher_precision *Node_k55[MAX_LEVELS];
  higher_precision *Node_k56[MAX_LEVELS];
  higher_precision *Node_k61[MAX_LEVELS];
  higher_precision *Node_k62[MAX_LEVELS];
  higher_precision *Node_k63[MAX_LEVELS];
  higher_precision *Node_k64[MAX_LEVELS];
  higher_precision *Node_k65[MAX_LEVELS];
  higher_precision *Node_k66[MAX_LEVELS];

  int *Node_map[MAX_LEVELS];
  int *Node_map_3[MAX_LEVELS];

  higher_precision *Bi; 
  higher_precision *BI[MAX_LEVELS];      /* inv of  diagonal elements of K matrix */
  higher_precision *Bi1,*Bi2,*Bi3;
  higher_precision *BI1[MAX_LEVELS];      /* inv of  diagonal elements of K matrix */
  higher_precision *BI2[MAX_LEVELS]; 
  higher_precision *BI3[MAX_LEVELS]; 
  higher_precision *BI4[MAX_LEVELS];      /* inv of  diagonal elements of K matrix */
  higher_precision *BI5[MAX_LEVELS]; 
  higher_precision *BI6[MAX_LEVELS]; 
  higher_precision *BDD1[MAX_LEVELS];      /* inv of  diagonal elements of K matrix */
  higher_precision *BDD2[MAX_LEVELS]; 
  higher_precision *BDD3[MAX_LEVELS]; 
  higher_precision *Bpi;  
  higher_precision *BPI[MAX_LEVELS];
  standard_precision *V[7],*V1[7];   /* velocity X[dirn][node] can save memory */

  char *update_elt_k[MAX_LEVELS];

  standard_precision *Pstrain;			/* (nodal) plastic strain */
  standard_precision *Pvisc;			/* tracer.Visc -> nodal_visc */
  higher_precision *H,*S;
  standard_precision *Psi;
  standard_precision *Q,*F,*Fb;			/* solid pressure, force vector, boundary forces */
  standard_precision *nQ,*NQ[MAX_LEVELS];	/* nodal solid pressure */
  standard_precision *Mass[MAX_LEVELS];		/* lumped mass matrix  for p-g solver etc. */  
  standard_precision *mass;			/* lumped mass matrix at levmax */
  standard_precision *Trmass[MAX_LEVELS];	/* lumped mass matrix  for p-g solver etc. */  
  standard_precision *trmass;               
  standard_precision *tw;
  standard_precision *x[4];			/* node coordinate at finest level (levmax) */
  standard_precision *X[MAX_LEVELS][4];		/* node coordinate at all levels */
  standard_precision *sx[4];			/* node curvilinear coordinate at levmax */
  standard_precision *SX[MAX_LEVELS][4]; 	/* node curvilinear coordinate at all levels */
  standard_precision *sv[7];
  standard_precision *VV[MAX_LEVELS][7];	/* velocity on all levels */
  standard_precision *output ;                  /* Variable needed for output computations */

  standard_precision *FF[MAX_LEVELS];		/* force vector at all levels */
  standard_precision *FFb[MAX_LEVELS];		/* boundary forces at all levels */
  standard_precision *QQ[MAX_LEVELS];		/* solid pressure at all levels */
  standard_precision *Vb[7][MAX_LEVELS];	/* velocity bc at all levels */
  standard_precision *Tb[MAX_LEVELS];		/* T bc at all levels, needed for solving v,p */

  standard_precision *edot,*strs,*strd,*depl,*melt,*strd_hor,*strd_ver;		/* (nodal) strain rate and stress */
                                                                            /* (nodal) depletion, RAA: C. O'Neill melting stuff */
  standard_precision *strd1;

  standard_precision *T,*T1,*Tdot;		/* T, dT/dt at finest level (levmax) */
  standard_precision *VB[7],*TB;		/* boundary conditions for V,T defined everywhere (levmax)*/
  standard_precision *TW[MAX_LEVELS];		/* nodal weightings */

  unsigned int *node;			/* properties of node at levmax */
  unsigned int *NODE[MAX_LEVELS];	/* node properties at all levels */
  unsigned int *elt;
     		
  int *index[MAX_LEVELS];

  struct ID *idd; 
  struct LM *lmd;
  
  struct Shape_function1 M;		/* master-element shape functions */
  struct Shape_function1_dx Mx;		/* master-element shape function gradients */
  struct Shape_function N;		/* element shape functions */
  struct Shape_function_dx Nx;		/* element shape function gradients */
  
    void (* iterative_solver)();
    void (* next_buoyancy_field)();
    void (* problem_settings)();
    void (* problem_derived_values)();
    void (* problem_allocate_vars)();
    void (* velocity_boundary_conditions)();
    void (* buoyancy_boundary_conditions)();    
    void (* problem_node_positions)();
    void (* problem_update_node_positions)();
    void (* problem_initial_fields)(); 
    void (* problem_update_bcs)();
    void (* special_process_new_velocity)();
    void (* special_process_new_buoyancy)();
    void (* solve_stokes_problem)(); 
    void (* solver_allocate_vars)(); 
    void (* transform)();
  
  standard_precision (* advection_source_term)(); 
  standard_precision (* node_space_function[3])(); 
};
