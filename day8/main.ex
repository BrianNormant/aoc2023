defmodule Solution do
  defp look_up(map, path, travel_left, "ZZZ", step) do
    step
  end
  defp look_up(map, path, [], current, step) do
    look_up(map, path, path, current, step)
  end
  defp look_up(map, path, travel_left, current, step) do
    step = step + 1
    [ next | travel_left ] = travel_left
    case Map.fetch(map, current) do
      {:ok, [ l, r ] } ->
        case next do
          "L" -> look_up(map, path, travel_left, l, step)
          "R" -> look_up(map, path, travel_left, r, step)
          _ -> {:error, "invalid route spefication: #{next}"}
        end
      _ -> {:error, "Could not find #{current} in the map"}
    end
  end

  def part1(c) do
    [route|map] = c |> String.split("\n", trim: true)
    route = route |> String.graphemes()
    map 
      |> Enum.map( fn s -> 
        s |> String.split(" = ", trim: true)
      end)
      |> Enum.reduce(%{}, fn ([ node, childs ], acc) ->
        childs = childs 
          |> String.replace("(", "")
          |> String.replace(")", "")
          |> String.replace(",", "")
          |> String.split(" ")
        Map.put(acc, node, childs)
      end)
      |> look_up(route, route, "AAA", 0)
      |> IO.inspect()
  end
  
  defp look_up2(map, path, [], current, step) do
    look_up2(map, path, path, current, step)
  end
  defp look_up2(map, path, travel_left, current, step) do
    if Regex.match?(~r/..Z/, current) do
      step
    else
      step = step + 1
      [ next | travel_left ] = travel_left
      case Map.fetch(map, current) do
        {:ok, [ l, r ] } ->
          case next do
            "L" -> look_up2(map, path, travel_left, l, step)
            "R" -> look_up2(map, path, travel_left, r, step)
            _ -> {:error, "invalid route spefication: #{next}"}
          end
        _ -> {:error, "Could not find #{current} in the map"}
      end
    end
  end
  
  defp ppcm(nums) do
    primes =
      case File.read("primes.txt") do
        {:ok, str} ->
          str
            |> String.split("\n", trim: true)
            |> Enum.map(fn s ->
              s |> Integer.parse()
                |> then(fn {d, _} -> d end)
            end)
        {:error} -> {:error, "Failed to parse primes"}
      end

    nums
      |> Enum.map( fn num -> 
        Enum.reduce(primes, {num, []}, fn (p, acc) ->
          {n, factors} = acc
          if rem(n, p) == 0 do
            {div(n, p), [p | factors]}
          else
            acc
          end
        end)
        |> then( fn {_, factors} -> factors end )
      end)
      |> Enum.flat_map( &(&1) )
      |> Enum.uniq()
      |> Enum.product()
  end
  
  def part2(c) do
    [route|map] = c |> String.split("\n", trim: true)
    route = route |> String.graphemes()
    map = map
      |> Enum.map( fn s -> 
        s |> String.split(" = ", trim: true)
      end)
      |> Enum.reduce(%{}, fn ([ node, childs ], acc) ->
        childs = childs 
          |> String.replace("(", "")
          |> String.replace(")", "")
          |> String.replace(",", "")
          |> String.split(" ")
        Map.put(acc, node, childs)
      end)
    map
      |> Map.keys()
      |> Enum.filter( &Regex.match?(~r/..A/, &1 ) )
      |> Enum.map(&look_up2(map, route, route, &1, 0) )
      |> ppcm()
      |> IO.inspect()
  end
end

# case File.read("example.txt") do
case File.read("puzzle.txt") do
  {:ok, content} ->
    # Solution.part1(content)
    Solution.part2(content)
  {:error} -> IO.put("Failed to read example.txt")
end

