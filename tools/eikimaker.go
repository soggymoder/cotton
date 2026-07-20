// i would have made this as a shell script but im more familiar with Go anyway so blehh
package main

import (
	"fmt"
)

func main() {

	fmt.Print("da character / letter : ")

	var ch string
	fmt.Scanln(&ch)

	fmt.Println("type a # for a pixel and a . for an empty spot")

	rows := make([]string, 5)

	for i := 0; i < 5; i++ {
		fmt.Printf("%d: ", i)
		fmt.Scanln(&rows[i])
	}

	fmt.Println()
	fmt.Printf("['%s'] = {\n", ch)

	for _, row := range rows {
		var value uint8

		for i, pixel := range row {
			if pixel == '#' {
				value |= 1 << i
			}
		}

		fmt.Printf("    0b%08b,\n", value)
	}

	fmt.Println("},")
}
