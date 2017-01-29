x = scan("ProductPrices.txt")
y = scan("PortfolioPrices.txt")
t = scan("time.txt")
plot(t,x,type="l",col="blue",xlim=c(0,8),ylim=c(95,130), ann=FALSE)

lines(t,y,col="red")
legend( x="topleft", 
         legend=c("Prix de l'Actigo","Prix du portefeuille de couverture"),
         col=c("blue","red"), pch = 20 , bty = "n" )
title(main = "Couverture de l'Actigo",
              xlab = "Temps ( en années)",
              ylab = "Valeur ( en euros)" )

dev.new()
x1 = scan("ESX50.txt")
x2 = scan("SSP500.txt")
x3 = scan("SSP200.txt")
x4 = scan("XED.txt")
x5 = scan("XEA.txt")
t = scan("time.txt")
plot(t,x1,type="l",col="blue",xlim=c(0,8),ylim=c(0.1,3))
lines(t,x2,col="red")
lines(t,x3,col="green")
lines(t,x4,col="orange")
lines(t,x5,col="brown")
legend( x="topleft", 
         legend=c("ESX50","SSP500","SSP200","EUR/USD","EUR/AUD"),
         col=c("blue","red","green","orange","brown"), pch = 20 , bty = "n" )
