x = scan("PerfPrices.txt")
y = scan("PortfolioPricesPerf.txt")
t = scan("timePerf.txt")
plot(t,x,type="l",col="blue")
lines(t,y,type="l",col="red")
legend( x="topleft", 
         legend=c("Prix de l'option performance","Prix du portefeuille de couverture"),
         col=c("blue","red"), pch = 20 , bty = "n" )
