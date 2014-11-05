void run_significance(  std::string mode,
			std::string channel,
			std::string inputdir,
			Double_t mass_low,
			Double_t mass_high,
			Double_t mass_step,
			Int_t ntoys = 1,
			std::string suffix,
			Double_t fit_range_low,
			Double_t fit_range_high,
			bool add_fake_data)
{
  gSystem->SetIncludePath( "-Itardir/");
  gSystem->Load("tardir/twobody_C.so");
  significance( mode,
		channel,
		inputdir,
		mass_low,
		mass_high,
		mass_step,
		ntoys,
		suffix,
		fit_range_low,
		fit_range_high,
		add_fake_data );
  return;
}
