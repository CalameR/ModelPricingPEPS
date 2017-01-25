x = scan("AsianPrices.txt")
y = scan("PortfolioPricesAsian.txt")
t = scan("timeAsian.txt")
plot(t,x,type="l",col="blue")
lines(t,y,type="l",col="red")
legend( x="topleft", 
         legend=c("Prix de l'option asiatique","Prix du portefeuille de couverture"),
         col=c("blue","red"), pch = 20 , bty = "n" )
