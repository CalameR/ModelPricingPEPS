x = scan("AsianPrices.txt")
y = scan("PortfolioPricesAsian.txt")
t = scan("timeAsian.txt")
plot(t,x,type="l",col="blue", ann=FALSE)
lines(t,y,type="l",col="red")
title(main = "Couverture de l'option asiatique",
              xlab = "Temps ( en années)",
              ylab = "Valeur ( en euros)" )
legend( x="topleft", 
         legend=c("Prix de l'option asiatique","Prix du portefeuille de couverture"),
         col=c("blue","red"), pch = 20 , bty = "n" )
