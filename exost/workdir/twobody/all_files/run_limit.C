void run_limit( std::string channel, // dimuon, dielectron etc
		std::string mode,    // obsereved, expected, mass limit (extra k-factor uncertainty)
		Float_t peak,        // resonance mass
		std::string suffix,  // suffix for output file names
		Int_t ntoys,         // number of pseudoexperiments for expected limit
		Int_t mcmc_iter,     // number of MCMC iterations
		Int_t mcmc_burnin,   // number of MCMC burn in steps to be discarded
		std::string inputdir,
		std::string plot_name )
{
  gSystem->SetIncludePath( "-Itardir/");
  gSystem->Load("tardir/twobody_C.so");
  limit( channel, // dimuon, dielectron etc
	 mode,    // obsereved, expected, mass limit (extra k-factor uncertainty)
	 peak,    // resonance mass
	 suffix,  // suffix for output file names
	 ntoys,   // number of pseudoexperiments for expected limit
	 mcmc_iter,   // number of MCMC iterations
	 mcmc_burnin, // number of MCMC burn in steps to be discarded
	 inputdir,
	 plot_name );
  return;
}
