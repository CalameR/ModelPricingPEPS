x = scan("QuantoPrices.txt")
y = scan("PortfolioPricesQuanto.txt")
t = scan("timeQuanto.txt")
plot(t,x,type="l",col="blue", ann=FALSE)
lines(t,y,type="l",col="red")
title(main = "Couverture d'une option quanto",
              xlab = "Temps ( en années)",
              ylab = "Valeur ( en euros)" )
legend( x="topleft", 
         legend=c("Prix de l'option quanto","Prix du portefeuille de couverture"),
         col=c("blue","red"), pch = 20 , bty = "n" )
