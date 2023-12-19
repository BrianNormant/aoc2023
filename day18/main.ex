defmodule Solution do
  
  defp pairs(l) do
    [f | _] = l
    l = l ++ [f]
    l |> Enum.chunk_every(2,1, :discard)
  end
  
  def shoelace_triangle(points2d) do
    points2d 
    |> pairs()
    |> Enum.reduce(0, fn([{m00, m10}, {m01, m11}], acc) -> 
      acc + m00 * m11 - m01 * m10
    end)
    |> then( fn a -> a/2 end)
  end

  defp parse(line) do
    line 
      |> String.split(" ")
      |> Enum.drop(-1)
      |> then( fn [dir, dist] -> 
        {dir, String.to_integer(dist)}
      end)
  end

  defp extract_points({dir, d}, {x, y, dist, points}) do
    case dir do
      "U" -> {x + d, y, dist+d, points ++ [{x,y}]}
      "D" -> {x - d, y, dist+d, points ++ [{x,y}]}
      "R" -> {x, y + d, dist+d, points ++ [{x,y}]}
      "L" -> {x, y - d, dist+d, points ++ [{x,y}]}
      _   -> :error
    end
  end

  def part1(c) do
    c |> String.split("\n", trim: true )
      |> Enum.map(&parse/1)
      |> Enum.reduce( {0, 0, 0, []}, &extract_points(&1, &2) )
      |> then( fn {_, _, b, points} ->
        shoelace_triangle(points) + b/2 + 1 end)
      |> IO.inspect()
  end
  defp parse2(line) do
    [_, line] = line |> String.split("#")
    {
      case (line |> String.graphemes() |> Enum.at(5)) do
        "0" -> "R"
        "1" -> "D"
        "2" -> "L"
        "3" -> "U"
      end ,
      line |> String.slice(0..4) |> String.to_integer(16)
    }
  end
  def part2(c) do
    c |> String.split("\n", trim: true )
      |> Enum.map(&parse2/1)
      |> Enum.reduce( {0, 0, 0, []}, &extract_points(&1, &2) )
      |> then( fn {_, _, b, points} ->
        shoelace_triangle(points) + b/2 + 1 end)
      |> IO.inspect()
  end
end

# case File.read("puzzle.txt") do
case File.read("example.txt") do
  {:ok, content} ->
    Solution.part1(content)
    Solution.part2(content)
  {:error} -> IO.put("Failed to read example.txt")
end
