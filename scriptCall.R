x = scan("CallPrices.txt")
y = scan("PortfolioPricesCall.txt")
t = scan("timeCall.txt")
plot(t,x,type="l",col="blue", ann=FALSE)
lines(t,y,type="l",col="red")
title(main = "Couverture d'un Call Vanille ",
              xlab = "Temps ( en années)",
              ylab = "Valeur ( en euros)" )
legend( x="topleft", 
         legend=c("Prix du Call","Prix du portefeuille de couverture"),
         col=c("blue","red"), pch = 20 , bty = "n" )
