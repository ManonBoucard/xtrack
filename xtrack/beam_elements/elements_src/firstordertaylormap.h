#ifndef XTRACK_FIRSTORDERTAYLORMAP_H
#define XTRACK_FIRSTORDERTAYLORMAP_H

/*gpufun*/
void FirstOrderTaylorMap_track_local_particle(FirstOrderTaylorMapData el, LocalParticle* part0){

    int64_t const radiation_flag = FirstOrderTaylorMapData_get_radiation_flag(el);
    double const length = FirstOrderTaylorMapData_get_length(el); // m

    //start_per_particle_block (part0->part)

        double x0 = LocalParticle_get_x(part);
        double px0 = LocalParticle_get_px(part);
        double y0 = LocalParticle_get_y(part);
        double py0 = LocalParticle_get_py(part);
        double beta0 = LocalParticle_get_beta0(part);
        double beta = LocalParticle_get_rvv(part)*beta0;
        double tau0 = LocalParticle_get_zeta(part)/beta;
        double ptau0 = LocalParticle_get_ptau(part);

        LocalParticle_set_x(part,FirstOrderTaylorMapData_get_m0(el,0) +
                            FirstOrderTaylorMapData_get_m1(el,0,0)*x0 +
                            FirstOrderTaylorMapData_get_m1(el,0,1)*px0 +
                            FirstOrderTaylorMapData_get_m1(el,0,2)*y0 +
                            FirstOrderTaylorMapData_get_m1(el,0,3)*py0 +
                            FirstOrderTaylorMapData_get_m1(el,0,4)*tau0 +
                            FirstOrderTaylorMapData_get_m1(el,0,5)*ptau0);
        LocalParticle_set_px(part,FirstOrderTaylorMapData_get_m0(el,1) +
                            FirstOrderTaylorMapData_get_m1(el,1,0)*x0 +
                            FirstOrderTaylorMapData_get_m1(el,1,1)*px0 +
                            FirstOrderTaylorMapData_get_m1(el,1,2)*y0 +
                            FirstOrderTaylorMapData_get_m1(el,1,3)*py0 +
                            FirstOrderTaylorMapData_get_m1(el,1,4)*tau0 +
                            FirstOrderTaylorMapData_get_m1(el,1,5)*ptau0);
        LocalParticle_set_y(part,FirstOrderTaylorMapData_get_m0(el,2) +
                            FirstOrderTaylorMapData_get_m1(el,2,0)*x0 +
                            FirstOrderTaylorMapData_get_m1(el,2,1)*px0 +
                            FirstOrderTaylorMapData_get_m1(el,2,2)*y0 +
                            FirstOrderTaylorMapData_get_m1(el,2,3)*py0 +
                            FirstOrderTaylorMapData_get_m1(el,2,4)*tau0 +
                            FirstOrderTaylorMapData_get_m1(el,2,5)*ptau0);
        LocalParticle_set_py(part,FirstOrderTaylorMapData_get_m0(el,3) +
                            FirstOrderTaylorMapData_get_m1(el,3,0)*x0 +
                            FirstOrderTaylorMapData_get_m1(el,3,1)*px0 +
                            FirstOrderTaylorMapData_get_m1(el,3,2)*y0 +
                            FirstOrderTaylorMapData_get_m1(el,3,3)*py0 +
                            FirstOrderTaylorMapData_get_m1(el,3,4)*tau0 +
                            FirstOrderTaylorMapData_get_m1(el,3,5)*ptau0);
        double tau = FirstOrderTaylorMapData_get_m0(el,4) +
                            FirstOrderTaylorMapData_get_m1(el,4,0)*x0 +
                            FirstOrderTaylorMapData_get_m1(el,4,1)*px0 +
                            FirstOrderTaylorMapData_get_m1(el,4,2)*y0 +
                            FirstOrderTaylorMapData_get_m1(el,4,3)*py0 +
                            FirstOrderTaylorMapData_get_m1(el,4,4)*tau0 +
                            FirstOrderTaylorMapData_get_m1(el,4,5)*ptau0;
        double ptau = FirstOrderTaylorMapData_get_m0(el,5) +
                            FirstOrderTaylorMapData_get_m1(el,5,0)*x0 +
                            FirstOrderTaylorMapData_get_m1(el,5,1)*px0 +
                            FirstOrderTaylorMapData_get_m1(el,5,2)*y0 +
                            FirstOrderTaylorMapData_get_m1(el,5,3)*py0 +
                            FirstOrderTaylorMapData_get_m1(el,5,4)*tau0 +
                            FirstOrderTaylorMapData_get_m1(el,5,5)*ptau0;

        LocalParticle_update_delta(part,sqrt(ptau*ptau + 2.0*ptau/beta0+1.0)-1.0);
        beta = LocalParticle_get_rvv(part)*beta0;
        LocalParticle_set_zeta(part,tau*beta);

        // Radiation
        if (radiation_flag > 0 && length > 0){
            double dpx = LocalParticle_get_px(part)-px0;
            double dpy = LocalParticle_get_py(part)-py0;
            double curv = sqrt(dpx*dpx+dpy*dpy);
            if (radiation_flag == 1){
                synrad_average_kick(part, curv, length);
            }
            else if (radiation_flag == 2){
                synrad_emit_photons(part, curv, length);
            }
        }

    //end_per_particle_block
}

#endif
