x = scan("CallPrices.txt")
y = scan("PortfolioPricesCall.txt")
t = scan("timeCall.txt")
plot(t,x,type="l",col="blue")
lines(t,y,type="l",col="red")
legend( x="topleft", 
         legend=c("Prix du Call","Prix du portefeuille de couverture"),
         col=c("blue","red"), pch = 20 , bty = "n" )
