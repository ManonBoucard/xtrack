#ifndef XTRACK_ELENS_H
#define XTRACK_ELENS_H

/*gpufun*/
void Elens_track_local_particle(ElensData el, LocalParticle* part0){

    double const elens_length = ElensData_get_elens_length(el);
    double const inner_radius = ElensData_get_inner_radius(el);
    double const outer_radius = ElensData_get_outer_radius(el);
    double const current = ElensData_get_current(el);
    double const voltage = ElensData_get_voltage(el);
    double const residual_kick_x = ElensData_get_residual_kick_x(el);
    double const residual_kick_y = ElensData_get_residual_kick_y(el);


    // double const cos_z = SRotationData_get_cos_z(el);

    //start_per_particle_block (part0->part)

      // EPSILON_0, PI, C_LIGHT,

        // electron mass
        double const EMASS  = 510998.928;
        // speed of light

        #if !defined( C_LIGHT )
            #define   C_LIGHT ( 299792458.0 )
        #endif /* !defined( C_LIGHT ) */

        #if !defined( EPSILON_0 )
            #define   EPSILON_0 (8.854187817620e-12)
        #endif /* !defined( EPSILON_0 ) */

        #if !defined( PI )
            #define PI (3.1415926535897932384626433832795028841971693993751)
        #endif /* !defined( PI ) */

        double x      = LocalParticle_get_x(part);
        double y      = LocalParticle_get_y(part);

        // delta
        // double delta  = LocalParticle_get_delta(part);
        // charge ratio: q/q0
        // double qratio = LocalParticle_get_charge_ratio(part);
        // chi = q/q0 * m0/m
        double const chi    = LocalParticle_get_chi(part);
        // reference particle momentum
        double const p0c    = LocalParticle_get_p0c(part);
        // particle momentum
        // double pc     = (1+delta)*(chi/qratio)*(p0c);
        // reference particle charge
        double const q0     = LocalParticle_get_q0(part);

        // rpp = P0/P
        double const rpp     = LocalParticle_get_rpp(part);


        // transverse radius
        double r      = sqrt(x*x + y*y);

        double rvv    = LocalParticle_get_rvv(part);
        double beta0  = LocalParticle_get_beta0(part);

        // # magnetic rigidity
        double const Brho0  = p0c/(q0*C_LIGHT);

        // # Electron properties
        // total electron energy
        double const etot_e       = voltage + EMASS;
        // // electron momentum
        double const p_e          = sqrt(etot_e*etot_e - EMASS*EMASS);
        // // relativistic beta of electron
        double const beta_e       = p_e/etot_e;
        //
        // // # relativistic beta  of protons
        double beta_p = rvv*beta0;

        // keep the formulas more compact
        double const r1 = inner_radius;
        double const r2 = outer_radius;

        // # geometric factor frr uniform distribution
        double frr = 0.;

        //
        if( r < r1 )
        {
          frr = 0.;
        }
        else if ( r > r2 )
        {
          frr = 1.;
        }
        else
        {
          frr = ((r*r - r1*r1)/(r2*r2 - r1*r1));
        }


        // # calculate the kick at r2 (maximum kick)
        double theta_max = ((1.0/(4.0*PI*EPSILON_0)));
        theta_max = theta_max*(2*elens_length*current);

        // for the moment: e-beam in the opposite direction from proton beam
        // generalize later
        theta_max = theta_max*(1+beta_e*beta_p);

        theta_max = theta_max/(outer_radius*Brho0*beta_e*beta_p);
        theta_max = theta_max/(C_LIGHT*C_LIGHT);
        // theta max is now completed
        // theta_max = (-1)*theta_max/(rpp*chi);


        // now the actual kick the particle receives

        double theta_pxpy = 0.;
        double dpx = 0.;
        double dpy = 0.;
        //


        if ( r > r1 )
        {
          theta_pxpy = (-1)*frr*theta_max*(outer_radius/r)*(1/(rpp*chi));
          dpx        = x*theta_pxpy/r;
          dpy        = y*theta_pxpy/r;
        }
        else
        {
          // if the particle is not inside the e-beam, it will only
          // be subject to the residual kick
          dpx = residual_kick_x;
          dpy = residual_kick_y;
        }


        LocalParticle_add_to_px(part, dpx );
        LocalParticle_add_to_py(part, dpy );

        // we can update the particle properties or add to the particle properties
        // LocalParticle_add_to_px(part, dpx);
        // LocalParticle_add_to_py(part, dpy);

        // LocalParticle_set_py(part, py_hat);
    //end_per_particle_block
}

#endif
