void run_diphoton_comb( Float_t peak,
			std::string mode,    // obsereved, expected, mass limit (extra k-factor uncertainty)
			std::string suffix,  // suffix for output file names
			Int_t ntoys,         // number of pseudoexperiments for expected limit
			Int_t mcmc_iter,     // number of MCMC iterations
			Int_t mcmc_burnin,   // number of MCMC burn in steps to be discarded
			std::string inputdir)
{
  gSystem->SetIncludePath( "-Itardir/");
  gSystem->Load("tardir/twobody_C.so");
  dilepton_diphoton_combine( peak,
			     mode,    // obsereved, expected, mass limit (extra k-factor uncertainty)
			     suffix,  // suffix for output file names
			     ntoys,   // number of pseudoexperiments for expected limit
			     mcmc_iter,   // number of MCMC iterations
			     mcmc_burnin, // number of MCMC burn in steps to be discarded
			     inputdir );
  return;
}
