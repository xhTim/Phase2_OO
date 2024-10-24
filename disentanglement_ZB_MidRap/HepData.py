# Generate HepData for sumbission

import ROOT
from hepdata_lib import Submission, Table, Uncertainty, Variable
import numbers as np

# a class to create a submission

class HepDataCreator:

	def __init__(self, name, infile, infile_sys):
		self.name = name
		self.infile = infile
		self.infile_sys = infile_sys
		self.data_dict,	self.data_sys_dict = self.read()

	def read(self):
        # Open the ROOT file and get the TTree
		root_file = ROOT.TFile(self.infile, "READ")
		keys = root_file.Get("keys")
		keys = [str(key) for key in keys]
		data_dict = {	key : list(root_file.Get(key)) for key in keys	}

		root_file_sys = ROOT.TFile(self.infile_sys, "READ")
		keys_sys = root_file_sys.Get("keys")
		keys_sys = [str(key) for key in keys_sys]
		data_dict_sys = {	key : list(root_file_sys.Get(key)) for key in keys_sys	}

		sort_keys = list(data_dict["W"])
		data_dict_sorted = dict()
		data_dict_sys_sorted = dict()

		#sort the data_dict_sorted by the values of W
		for [key, value] in data_dict.items():
			if len(value) != 6:
				data_dict_sorted[key] = value
				continue
			if key == "W":
				data_dict_sorted[key] = sorted(value)
				continue
			zipped = zip(value,	sort_keys)
			data_dict_sorted[key] = [x for x,y in sorted(zipped, key=lambda x: x[1])]

		#sort the data_dict_sys_sorted by the values of W
		for [key, value] in data_dict_sys.items():
			if len(value) != 6:
				data_dict_sys_sorted[key] = value
				continue
			if key == "W":
				data_dict_sys_sorted[key] = sorted(value)
				continue
			zipped = zip(value,	sort_keys)
			data_dict_sys_sorted[key] = [x for x,y in sorted(zipped, key=lambda x: x[1])]

		# print(data_dict_sorted["Sigma"])
		# print(data_dict_sorted["W"])
		# print(data_dict_sorted["Dy"])
		# print(data_dict_sorted["DSigmaDy_AnAn"])
		# print(data_dict_sorted)

		# return data_dict, data_dict_sys
		return data_dict_sorted, data_dict_sys_sorted

	def create_variables(self, param, param_name="N/A", qualifier = None, independent=False, binned=False, units="", sys_breakdown=False):
		var = Variable(param_name, is_independent=independent, is_binned=binned, units=units)
		var.add_qualifier("SQRT(S)/NUCLEON", "5020", "GeV")
		var.add_qualifier("RE", "PB PB -> J/PSI <MU+ MU-> PB PB")
		var.add_qualifier("ABS(YRAP)", "1.6 - 2.4")
		var.add_qualifier("MUON ABS(ETA)", "0.0 - 2.4")

		n_sig_figs = 2
		# set the error to the same number of significant figures as the value
		errors = [f"{i:#.{n_sig_figs}g}" for i in self.data_dict[param+"_Err"]]
		# first get the number of decimal places in the error
		n_decimals = [len(str(i).split(".")[1]) for i in errors]
		if qualifier != None:
			var.add_qualifier("NEUTRON MULT", qualifier)
		# set the decimal places to the same as the error
		var.values = [ f"{i:.{j}f}" for i, j in zip(self.data_dict[param], n_decimals)]
		unc = Uncertainty("stat", is_symmetric=True)
		# around the number to 2 significant digits 
		unc.values = [ f"{i:.{j}f}" for i,j in zip(self.data_dict[param+"_Err"], n_decimals)]
		var.add_uncertainty(unc)

		if sys_breakdown:
			sys_thoery_unc = Uncertainty("syst_theory", is_symmetric=True)
			# sys_thoery_unc.values = self.data_sys_dict[param+"_TheorySysErr"]
			sys_thoery_unc.values = [f"{i:.{j}f}" for i,j in zip(self.data_sys_dict[param+"_TheorySysErr"], n_decimals)]
			var.add_uncertainty(sys_thoery_unc)

			sys_Experiment_unc = Uncertainty("syst_experiment", is_symmetric=True)
			# sys_Experiment_unc.values = self.data_sys_dict[param+"_ExperiSysErr"]
			sys_Experiment_unc.values = [f"{i:.{j}f}" for i,j in zip(self.data_sys_dict[param+"_ExperiSysErr"], n_decimals)]
			var.add_uncertainty(sys_Experiment_unc)

		else:
			sys_unc = Uncertainty("syst", is_symmetric=True)
			# sys_unc.values = self.data_sys_dict[param+"_SysErr"]
			sys_unc.values = [f"{i:.{j}f}" for i,j in zip(self.data_sys_dict[param+"_SysErr"], n_decimals)]
			var.add_uncertainty(sys_unc)

		return var


	def create_table(self):

		Dy_var = Variable("|$y$|", is_independent=True, is_binned=True, units="")
		# Dy_var.values = self.data_dict["Dy"]
		# Dy_var.low = [y-dy for y, dy in zip(self.data_dict["Dy"], self.data_dict["Dy_Err"])]
		# Dy_var.high = [y+dy for y, dy in zip(self.data_dict["Dy"], self.data_dict["Dy_Err"])]
		Dy_var.values = [(y-dy, y+dy) for y, dy in zip(self.data_dict["Dy"], self.data_dict["Dy_Err"])]

		# Dy_unc = Uncertainty("Dy_stat_unc", is_symmetric=True)
		# Dy_unc.values = self.data_dict["Dy_Err"]
		# print(self.data_dict["Dy_Err"])
		# Dy_var.add_uncertainty(Dy_unc)

		DSigmaDy_AnAn_var 		= self.create_variables("DSigmaDy_AnAn",	"$\\frac{d\sigma_{\mathrm{J}/\psi}^{\mathrm{AnAn}}}{dy}$", units="mb", qualifier="AnAn")
		DSigmaDy_0n0n_var 		= self.create_variables("DSigmaDy_0n0n",	"$\\frac{d\sigma_{\mathrm{J}/\psi}^{\mathrm{0n0n}}}{dy}$", units="mb", qualifier="0n0n")
		DSigmaDy_0nXnSum_var 	= self.create_variables("DSigmaDy_0nXnSum",	"$\\frac{d\sigma_{\mathrm{J}/\psi}^{\mathrm{0nXn}}}{dy}$", units="mb", qualifier="0nXn")
		DSigmaDy_XnXn_var 		= self.create_variables("DSigmaDy_XnXn", 	"$\\frac{d\sigma_{\mathrm{J}/\psi}^{\mathrm{XnXn}}}{dy}$", units="mb", qualifier="XnXn")

		DSigmaDy_table = Table("Table 1")
		DSigmaDy_table.description = "The differential coherent  $\mathrm{J}/\psi$ photoproduction cross section as a function of rapidity, in different neutron multiplicity classes: 0n0n, 0nXn, XnXn , and AnAn."
		DSigmaDy_table.location = "Data from Figure 2"
		DSigmaDy_table.add_variable(Dy_var)
		DSigmaDy_table.add_variable(DSigmaDy_AnAn_var)
		DSigmaDy_table.add_variable(DSigmaDy_0n0n_var)
		DSigmaDy_table.add_variable(DSigmaDy_0nXnSum_var)
		DSigmaDy_table.add_variable(DSigmaDy_XnXn_var)

		DSigmaDy_table.keywords["observables"] = ["DSIG/DYRAP"]


		W_var = Variable("$W_{\gamma \mathrm{N}}^{\mathrm{Pb}}$", is_independent=True, is_binned=False, units="GeV")
		# W_var.values = self.data_dict["W"]
		W_var.values = [float(f"{i:.2e}") for i in self.data_dict["W"]]

		Sigma_var = self.create_variables("Sigma",	"$\sigma(\gamma \mathrm{Pb} \\rightarrow \mathrm{J}/\psi \mathrm{Pb})$", units="mb", sys_breakdown=True)

		Sigma_table = Table("Table 2")
		Sigma_table.description = "The total coherent $\mathrm{J}/\psi$ photoproduction cross section as a function of photon-nuclear center-of-mass energy per nucleon $W_{\gamma \mathrm{N}}^{\mathrm{Pb}}$, measured in PbPb ultra-peripheral collisions at $\sqrt{s_{\mathrm{NN}}}$ = 5.02 TeV. The $W_{\gamma \mathrm{N}}^{\mathrm{Pb}}$ values used correspond to the center of each rapidity range. The theoretical uncertainties is due to the uncertainties in the photon flux."
		Sigma_table.location = "Data from Figure 3"
		Sigma_table.add_variable(W_var)
		Sigma_table.add_variable(Sigma_var)

		Sigma_table.keywords["observables"] = ["SIG"]


		X_var = Variable("$x$", is_independent=True, is_binned=False, units="")
		# X_var.values = self.data_dict["X"]
		X_var.values = [float(f"{i:.2e}") for i in self.data_dict["X"]]

		R_var = self.create_variables("R", "$R_{\mathrm{g}}^{\mathrm{Pb}}$", sys_breakdown=True)

		R_table = Table("Table 3")
		R_table.description = "The nuclear gluon suppression factor $R_{\mathrm{g}}^{\mathrm{Pb}}$ as a function of Bjorken $x$ extracted from the CMS measurement of the coherent $\mathrm{J}/\psi$ photoproduction in PbPb ultra-peripheral collisions at $\sqrt{s_{\mathrm{NN}}}$ = 5.02 TeV. The $x$ values are evaluated at the centers of their corresponding rapidity ranges. The theoretical uncertainties are due to the uncertainties in the photon flux and the  impulse approximation model."
		R_table.location = "Data from Figure 4"
		R_table.add_variable(X_var)
		R_table.add_variable(R_var)

		R_table.keywords["observables"] = ["R"]

		return [DSigmaDy_table, Sigma_table, R_table]


class HepDataCreatorMatrix:
	def __init__(self, name, infile):
		self.name = name
		self.infile = infile
		self.matrix = self.read()
		self.nrows = len(self.matrix)
		self.ncols = len(self.matrix[0])

	def read(self):
		#read a matrix from a text file
		#the first line is the header, showing the number of rows and columns
		matrix = []
		with open(self.infile, "r") as f:
			header = f.readline()
			header = header.split()
			nrows = int(header[0])
			ncols = int(header[1])

			#read the data
			for i in range(nrows):
				line = f.readline()
				line = line.split()
				line = [float(x) for x in line]
				matrix.append(list(line))

		return matrix


	def create_table(self, table_name="Total covariance matrix", table_description="N/A", table_location="N/A"):
		# Create the table object and add the variables
		x = []
		y = []
		for i in range(self.nrows):
			for j in range(self.ncols):
				x.append(i+1)
				y.append(j+1)

		#flatten the matrix
		z = [j for sub in self.matrix for j in sub]

		x_var = Variable("First bin", is_independent=True, is_binned=False, units="")
		x_var.values = x

		y_var = Variable("Second bin", is_independent=True, is_binned=False, units="")
		y_var.values = y

		z_var = Variable("Covariance", is_independent=False, is_binned=False, units="")
		z_var.values = z

		table = Table(table_name)
		table.description = table_description
		table.location = table_location
		for var in [x_var,y_var,z_var]:
			table.add_variable(var)

		table.keywords["observables"] = ["COV"]
		return [table]

def submit(tables_):
	# Submit the table to the HEPData database
	tables = list(tables_)
	submission = Submission()
	for table in tables:
		submission.add_table(table)

	submission.create_files("./outFiles/HepData", remove_old=True)

if (__name__ == "__main__"):
	print("HEPData.py")
	hep = HepDataCreator("test", "./outFiles/Result_CMS.root", 	"./outFiles/Result_CMS_SysUncer.root")
	hep_matrix = HepDataCreatorMatrix("test", "./outFiles/CovMatrix.txt")
	hep_matrix_experi = HepDataCreatorMatrix("test", "./outFiles/CovMatrixExperi.txt")
	hep_matrix_flux = HepDataCreatorMatrix("test", "./outFiles/CovMatrixFlux.txt")

	data_tables = hep.create_table()
	matrix_table = hep_matrix.create_table(table_description="The total covariance matrix of the total coherent photoproduction cross section as a function of photon-nuclear center-of-mass energy per nucleon $W_{\gamma \mathrm{N}}^{\mathrm{Pb}}$. The covariance matrix includes both the experimental and theoretical (photon flux) uncertainties. The bins are ordered as increasing in $W_{\gamma \mathrm{N}}^{\mathrm{Pb}}$.", 
										table_location="Total covariance matrix for data from Figure 3")
	matrix_table_experi = hep_matrix_experi.create_table(table_name="Experimental covariance matrix", 
						      			table_description="The experimental covariance matrix of the total coherent photoproduction cross section as a function of photon-nuclear center-of-mass energy per nucleon $W_{\gamma \mathrm{N}}^{\mathrm{Pb}}$. The bins are ordered as increasing in $W_{\gamma \mathrm{N}}^{\mathrm{Pb}}$.",
										table_location="Experimental covariance matrix for data from Figure 3")
	matrix_table_flux = hep_matrix_flux.create_table(table_name="Theoretical covariance matrix", 
						  				table_description="The theoretical (photon flux) covariance matrix of the total coherent photoproduction cross section as a function of photon-nuclear center-of-mass energy per nucleon $W_{\gamma \mathrm{N}}^{\mathrm{Pb}}$. The bins are ordered as increasing in $W_{\gamma \mathrm{N}}^{\mathrm{Pb}}$.",
										table_location="Theoretical covariance matrix for data from Figure 3")

	tables = list(data_tables + matrix_table + matrix_table_experi + matrix_table_flux)

	for t in tables:
		t.keywords["cmenergies"] = [5020]
		t.keywords["reactions"] = ["Pb Pb --> J/psi Pb Pb"]
		t.keywords["phrases"] = ["J/psi", "Photoproduction", "Suppression", "Nuclear Shadowing", "Cross Section"]
	submit(tables)
