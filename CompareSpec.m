% compare spectra from jonswapSpec
 clf;
 dat = load('jonswap_spec.txt', '-ascii');
 dat0= load('jonswap_sample.txt','-ascii');
 plot(dat(:,1),dat(:,2))
 hold on;
 plot(dat0(:,1),dat0(:,2),'+');