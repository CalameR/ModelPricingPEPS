x = scan("ProductPrices.txt")
t = scan("time.txt")
plot(t,x,type="l",col="blue",xlim=c(0,8),ylim=c(85,115))
legend( x="topleft", 
         legend=c("Prix du produit","Prix du portefeuille de couverture"),
         col=c("blue","red"), pch = 20 , bty = "n" )

dev.new()
x1 = scan("ESX50.txt")
x2 = scan("SSP500.txt")
x3 = scan("SSP200.txt")
x4 = scan("XED.txt")
x5 = scan("XEA.txt")
t = scan("time.txt")
plot(t,x1,type="l",col="blue",xlim=c(0,8),ylim=c(0.1,1.9))
lines(t,x2,col="red")
lines(t,x3,col="green")
lines(t,x4,col="orange")
lines(t,x5,col="brown")
legend( x="topleft", 
         legend=c("ESX50","SSP500","SSP200","EUR/USD","EUR/AUD"),
         col=c("blue","red","green","orange","brown"), pch = 20 , bty = "n" )
