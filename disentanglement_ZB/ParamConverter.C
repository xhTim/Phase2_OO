#ifndef PARAMCONVERTER_H
#define PARAMCONVERTER_H

#include "../common_ZB/constants.h"

// This file contains some useful functions to convert between different variables
struct ParamConverter
{
    static constexpr double Sqrt_s     = 7000;
    static constexpr double Gamma_beam = 3730.3;
    static constexpr double Mass_N     = (0.93827+0.93957) / 2;
    // the effective Lorentz factor in the rest frame of the collision partner A. It is related to that in the c.m. system of the two colliding heavy ions
    // Eq.4.2 Prog. Parr. Nucl. Phys.. Vol. 39, pp. 503-564. 1997
    static constexpr double Gamma_eff  = 2 * Gamma_beam * Gamma_beam -1;

	static double x2W(const double x)
	{
		return sqrt( pow(mJpsi_PDG,2 ) / x );
	}

	static double W2x(const double W)
	{
		return pow(mJpsi_PDG,2 ) / pow(W,2);
	}

	static double y2x(const double y)
	{
		return (mJpsi_PDG / Sqrt_s) * exp(-y);
	}
	
	static double y2W(const double y)
	{
		return sqrt( (2 * Gamma_beam * Mass_N * mJpsi_PDG) * exp(y) );
	}

	static double W2y(const double W)
	{
		return log( W * W / (2 * Gamma_beam * Mass_N * mJpsi_PDG) );
	}

	static double y2w(const double y)
	{
		//Rap to photon energy
		return mJpsi_PDG / 2 * exp(y);
	}

	static double y2w_eff(const double y)
	{
		//Rap to photon energy in A rest frame
		return w2w_eff( y2w(y) );
	}

	static double w2y(const double w)
	{
		//Photon energy to Rap
		return log(2 * w /mJpsi_PDG);
	}

	static double w2w_eff(const double w)
	{
		//convert photon energy in gamma-A cm frame to A rest frame
		return w * Gamma_eff;
	}

	static std::vector<double> W2w_eff_v( std::vector<double> W)
	{
		std::vector<double> v_;
		//convert photon energy in gamma-A cm frame to A rest frame
		for (int i = 0; i < W.size(); i++)
		{
			v_.push_back( y2w_eff(W2y( W[i])) );
		}
		return v_;
	}
};

#endif
